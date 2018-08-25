#ifndef PLAYLISTVIEW_H
#define PLAYLISTVIEW_H

#include <QTableView>
#include <QHeaderView>
#include <QMenu>
#include <XYZPlayer/MusicPlayer.h>

class PlaylistView : public QTableView
{
public:
	PlaylistView(QWidget* parent=nullptr);
private:
	QMenu* mPopupMenu;
};

#endif // PLAYLISTVIEW_H
