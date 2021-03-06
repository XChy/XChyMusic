#ifndef SONGLISTSMODEL_H
#define SONGLISTSMODEL_H

#include <QAbstractListModel>
#include <QMimeData>
#include <QIcon>
#include <QDebug>
#include <XYZPlayer/Songlist.h>
#include <XYZPlayer/MusicLoader.h>

class SonglistsModel : public QAbstractListModel
{
		Q_OBJECT
	public:
		SonglistsModel(QObject* parent=nullptr);

		virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
		virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

		virtual Qt::DropActions supportedDropActions() const override;
		virtual Qt::DropActions supportedDragActions() const override;

		virtual Qt::ItemFlags flags(const QModelIndex &index) const override;

		virtual QStringList mimeTypes() const override;

		virtual QMimeData* mimeData(const QModelIndexList &indexes) const override;

		virtual bool canDropMimeData(const QMimeData *data, Qt::DropAction action,
									 int row, int column, const QModelIndex &parent) const override;
		virtual bool dropMimeData(const QMimeData *data,Qt::DropAction action, int row, int column, const QModelIndex &parent) override;

		void refresh();

		Songlists* songlists() const;
		void setSonglists(Songlists* songlists);
	public slots:
//		void onPictureLoaded(int index);
	private:
		Songlists* _songlists;

};

#endif // SONGLISTSMODEL_H
