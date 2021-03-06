#include "PlaylistModel.h"
#include <QDebug>

PlaylistModel::PlaylistModel(QObject* parent)
	:QAbstractTableModel(parent),
	  _player(nullptr)
{

}

int PlaylistModel::rowCount(const QModelIndex& parent) const
{
	if(parent.isValid())
		return 0;

	return _player->playlist().size();
}

int PlaylistModel::columnCount(const QModelIndex& parent) const
{
	if(parent.isValid())
		return 0;

	return 4;
}

QVariant PlaylistModel::data(const QModelIndex& index, int role) const
{
	if(role==Qt::DisplayRole){
		if(!index.isValid()){
			return QVariant();
		}
		switch(index.column()){
		case 0:
			return _player->currentIndex()==index.row();
		case 1:
			return _player->playlist()[index.row()].infoTags["title"];
		case 2:
			return _player->playlist()[index.row()].infoTags["artist"];
		case 3:
			return _player->playlist()[index.row()].duration;
		}
	}
	return QVariant();
}

Qt::DropActions PlaylistModel::supportedDropActions() const
{
	return Qt::CopyAction | Qt::MoveAction;
}

Qt::DropActions PlaylistModel::supportedDragActions() const
{
	return Qt::MoveAction;
}

Qt::ItemFlags PlaylistModel::flags(const QModelIndex& index) const
{
	Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);
	return Qt::ItemIsDropEnabled| Qt::ItemIsDragEnabled | defaultFlags;
}

QStringList PlaylistModel::mimeTypes() const
{
	return {"text/url-list","XYZPlayer/MusiclistAndIndexes"};
}

QMimeData* PlaylistModel::mimeData(const QModelIndexList& indexes) const
{
	QMimeData* data=new QMimeData;
	QByteArray array;
	QDataStream stream(&array,QIODevice::WriteOnly);
	stream<<(qulonglong)&_player->playlist();
	stream<<indexes.size()/columnCount();
	for(int i=0;i<indexes.size()/columnCount();++i){
		stream<<indexes[i*columnCount()].row();
	}

	data->setData("XYZPlayer/MusiclistAndIndexes",array);
	return data;
}

bool PlaylistModel::canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) const
{
	return data->hasUrls()||data->hasFormat("XYZPlayer/MusiclistAndIndexes");
}

bool PlaylistModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent){
	qDebug()<<"Drop in playlist:"<<row<<", Parent"<<parent;
	if (action == Qt::IgnoreAction)
		return true;

	int insertIndex;
	if (row != -1)
		insertIndex = row;
	else if (parent.isValid())
		insertIndex = parent.row();
	else
		insertIndex = rowCount();

	if (data->hasUrls()){

		int beginIndex=insertIndex;

		for(QUrl url:data->urls()){
			MusicObject obj;
			obj.path=url.toLocalFile();
			obj.lyrics.path=url.toLocalFile().left(url.toLocalFile().lastIndexOf('.')).append(".lrc");
			_player->insertMusic(insertIndex,obj);
			++insertIndex;
		}
		_player->asyncLoadInfo(beginIndex,insertIndex);
		_player->playAt(beginIndex);
		return true;
	}
	if(data->hasFormat("XYZPlayer/MusiclistAndIndexes")){
		QByteArray array=data->data("XYZPlayer/MusiclistAndIndexes");
		QDataStream stream(&array,QIODevice::ReadOnly);
		QList<MusicObject>* list;
		int size;
		stream>>(qulonglong&)list;
		stream>>size;
		qDebug()<<"Insert in playlist:"<<insertIndex;

		if(list == &_player->playlist()){
			if(insertIndex==rowCount())
				--insertIndex;
			QList<int> indexes;
			for(int i=0;i<size;++i){
				int index;
				stream>>index;
				indexes<<index;
			}
			std::sort(indexes.begin(),indexes.end());
			for(int i=0;i<size;++i){
				_player->moveMusic(indexes[i],insertIndex);
				if(indexes[i]>=insertIndex){
					++insertIndex;
				}
			}
		}else{
			for(int i=0;i<size;++i){
				int index;
				stream>>index;
				_player->insertMusic(insertIndex,list->at(index));
			}
		}

		return true;
	}

	return QAbstractTableModel::dropMimeData(data,action,row,column,parent);
}

void PlaylistModel::onCurrentIndexChanged(int oldIndex, int newIndex)
{
	if(oldIndex==newIndex)return;
	if(oldIndex==-1){
		emit dataChanged(index(newIndex,0),index(newIndex,0));
	}else if(newIndex==-1){
		emit dataChanged(index(oldIndex,0),index(oldIndex,0));
	}else{
		emit dataChanged(index(oldIndex,0),index(oldIndex,0));
		emit dataChanged(index(newIndex,0),index(newIndex,0));
	}
}

void PlaylistModel::onInfoLoaded(int index)
{
	emit dataChanged(this->index(index,1),this->index(index,3));
}

void PlaylistModel::refresh()
{
	beginResetModel();
	endResetModel();
}

MusicPlayer* PlaylistModel::player() const
{
	return _player;
}

void PlaylistModel::setPlayer(MusicPlayer* player)
{
	if(player){
		connect(player,&MusicPlayer::currentIndexChanged,this,&PlaylistModel::onCurrentIndexChanged);
		connect(player,&MusicPlayer::infoLoaded,this,&PlaylistModel::onInfoLoaded);
		connect(player,&MusicPlayer::playlistElementsChanged,this,&PlaylistModel::refresh);
	}
	if(_player){
		disconnect(_player,&MusicPlayer::currentIndexChanged,this,&PlaylistModel::onCurrentIndexChanged);
		disconnect(_player,&MusicPlayer::infoLoaded,this,&PlaylistModel::onInfoLoaded);
		disconnect(_player,&MusicPlayer::playlistElementsChanged,this,&PlaylistModel::refresh);
	}
	_player = player;
}
