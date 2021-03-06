#include "SonglistModel.h"

SonglistModel::SonglistModel(QObject *parent)
	: QAbstractTableModel(parent),
	  _songlist(nullptr)
{
}

int SonglistModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid()||!_songlist)
		return 0;

	return _songlist->size();
}

int SonglistModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid()||!_songlist)
		return 0;

	return 4;
}

QVariant SonglistModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if(role==Qt::DisplayRole){
		switch (section) {
		case 0:
			return tr("title");
		case 1:
			return tr("artist");
		case 2:
			return tr("album");
		case 3:
			return tr("duration");
		}
	}
	return QAbstractTableModel::headerData(section,orientation,role);
}

QVariant SonglistModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();
	if(!_songlist)
		return QVariant();

	if(role==Qt::DisplayRole){
		switch (index.column()) {
		case 0:
			return _songlist->at(index.row()).infoTags["title"];
		case 1:
			return  _songlist->at(index.row()).infoTags["artist"];
		case 2:
			return _songlist->at(index.row()).infoTags["album"];
		case 3:
			return _songlist->at(index.row()).duration;
		}
	}
	return QVariant();
}

Qt::DropActions SonglistModel::supportedDropActions() const
{
	return Qt::CopyAction | Qt::MoveAction;
}

Qt::DropActions SonglistModel::supportedDragActions() const
{
	return Qt::MoveAction;
}

Qt::ItemFlags SonglistModel::flags(const QModelIndex& index) const
{
	Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);
	return Qt::ItemIsDropEnabled| Qt::ItemIsDragEnabled | defaultFlags;
}

QStringList SonglistModel::mimeTypes() const
{
	return {"text/url-list","XYZPlayer/MusiclistAndIndexes"};
}

QMimeData* SonglistModel::mimeData(const QModelIndexList& indexes) const
{
	QMimeData* data=new QMimeData;
	QByteArray array;
	QDataStream stream(&array,QIODevice::WriteOnly);
	stream<<(qulonglong)_songlist;
	stream<<indexes.size()/columnCount();
	for(int i=0;i<indexes.size()/columnCount();++i){
		stream<<indexes[i*columnCount()].row();
	}

	data->setData("XYZPlayer/MusiclistAndIndexes",array);
	return data;
}

bool SonglistModel::canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) const
{
	return data->hasUrls()||data->hasFormat("XYZPlayer/MusiclistAndIndexes");
}

bool SonglistModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
	return QAbstractTableModel::dropMimeData(data,action,row,column,parent);
}

void SonglistModel::refresh()
{
	beginResetModel();
	endResetModel();
}

Songlist* SonglistModel::songlist() const
{
	return _songlist;
}

void SonglistModel::setSonglist(Songlist* songlist)
{
	_songlist = songlist;
	refresh();
}
