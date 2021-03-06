#include "ItemDelegates.h"
#include <QDebug>

PlayingItemDelegate::PlayingItemDelegate(QObject* parent)
	:QItemDelegate(parent)
{}

void PlayingItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	drawBackground(painter,option,index);
	if(index.data().toBool()){
		painter->setRenderHint(QPainter::SmoothPixmapTransform);
		painter->drawImage(option.rect,QImage(":/images/easy/playing.png"));
	}
}

TitleItemDelegate::TitleItemDelegate(QObject* parent)
	:QItemDelegate(parent)
{}

void TitleItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	drawBackground(painter,option,index);
	painter->setRenderHint(QPainter::TextAntialiasing);
	painter->setPen(Qt::black);
	painter->setFont(QFont("微软雅黑"));
	QTextOption textOption;
	textOption.setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
	painter->drawText(option.rect,elidedText(
						  painter->fontMetrics(),option.rect.width(),Qt::ElideRight,index.data().toString()),textOption);
}

ArtistItemDelegate::ArtistItemDelegate(QObject* parent)
	:QItemDelegate(parent)
{}

void ArtistItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	drawBackground(painter,option,index);
	painter->setRenderHint(QPainter::TextAntialiasing);
	painter->setPen(QColor(0x66,0x66,0x66));
	painter->setFont(QFont("微软雅黑"));
	QTextOption textOption;
	textOption.setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
	painter->drawText(option.rect,elidedText(
						  painter->fontMetrics(),option.rect.width(),Qt::ElideRight,index.data().toString()),textOption);
}

DurationItemDelegate::DurationItemDelegate(QObject* parent)
	:QItemDelegate(parent)
{}

void DurationItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	drawBackground(painter,option,index);
	painter->setRenderHint(QPainter::TextAntialiasing);
	painter->setPen(QColor(0x50,0x50,0x50));
	painter->setFont(QFont("微软雅黑"));
	QTextOption textOption;
	textOption.setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);

	int64_t duration=index.data().toLongLong();

	painter->drawText(option.rect,elidedText(
						  painter->fontMetrics(),
						  option.rect.width(),Qt::ElideRight,
						  QTime(0,0).addSecs(duration/1000000).toString("m:ss")),
					  textOption);
}

LyricItemDelegate::LyricItemDelegate(QObject* parent)
	:QItemDelegate(parent)
{

}

void LyricItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	drawBackground(painter,option,index);
	painter->setRenderHint(QPainter::TextAntialiasing);

	if(index.data(Qt::UserRole).toBool()){
		painter->setPen(QColor(0x00,0xa1,0xec));
	}else{
		painter->setPen(QColor(0x66,0x66,0x66));
	}
	painter->setFont(QFont("微软雅黑"));
	QTextOption textOption;
	textOption.setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
	painter->drawText(option.rect,index.data().toString(),textOption);
}
