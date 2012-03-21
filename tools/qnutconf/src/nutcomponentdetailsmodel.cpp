
#include "nutcomponentmodel.h"
#include "nutcomponentmodel_p.h"
#include "nutcomponentdetailsmodel.h"

/*!
	\class NutComponentDetailsModel is a Proxy Model to NutComponentModel, presenting details of
	a given item using the refresh method.
*/

NutComponentDetailsModel::NutComponentDetailsModel( NutComponentModel* parent ) : QAbstractTableModel( parent ), parentModel( parent )
{
}

QVariant NutComponentDetailsModel::headerData( int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole */ ) const
{
	if ( orientation != Qt::Horizontal )
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	switch( section )
	{
	case 0:	return tr("Property");
	case 1: return tr("Value");
	default: break;
	}

	return QVariant();
}

QVariant NutComponentDetailsModel::data( const QModelIndex& index, int role /*= Qt::DisplayRole */ ) const
{
	if ( role == Qt::DisplayRole )
	{
		QStringList row = cache.at( index.row() );
		return row.at( index.column() );
	}
	return QVariant();
}

int NutComponentDetailsModel::rowCount( const QModelIndex& parent /*= QModelIndex() */ ) const
{
	return cache.count();
}

int NutComponentDetailsModel::columnCount( const QModelIndex& parent /*= QModelIndex() */ ) const
{
	return 2;
}

/*!
	Retrieve from parent model details for a given item.
*/
void NutComponentDetailsModel::refresh( const QModelIndex& selected )
{
#if QT_VERSION >= 0x040600
	beginResetModel();
#endif

	cache.clear();
	if ( !selected.isValid() )
		return;

	TreeItem *item = static_cast<TreeItem*>(selected.internalPointer());
	cache.append( QStringList() << tr("Enabled") << (item->isEnabled() ? tr("Yes") : tr("No")) );

	QString value;
	value = item->depends().join(", ");
	if ( !value.isEmpty() )
		cache.append( QStringList() << tr("Requires") << value );

	value = item->provides().join(", ");
	if ( !value.isEmpty() )
		cache.append( QStringList() << tr("Provides") << value );

	if ( item->componentType() == TreeItem::nutOption )
	{
		cache.append( QStringList() << tr("File") << item->headerFile() );
		cache.append( QStringList() << tr("Macro") << item->name() );
		cache.append( QStringList() << tr("Active") << (item->isActive() ? tr("Yes") : tr("No")) );
	}

#if QT_VERSION >= 0x040600
	endResetModel();
#else
	reset();
#endif
}
