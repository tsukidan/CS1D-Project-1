#include "sqldatabase.h"

/*!
 * \brief Constructor for sqlDatabase
 * \param parent
 */

SQLDatabase::SQLDatabase()
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    QFileInfo db("database.sqlite");
    QString DB_PATH = db.absoluteFilePath();
    qDebug() << DB_PATH;
    database.setDatabaseName(DB_PATH);
    database.open();
}
/*!
 * \brief Destructor for sqlDatabase
 */
SQLDatabase::~SQLDatabase()
{

}
/*!
 * \brief Creates the Database
 */
void SQLDatabase::createDatabase()
{
    QSqlQuery query;
    query.exec("CREATE TABLE   CustomerTable("
               "Name           VARCHAR(50),"
               "CustomerID     INTEGER NOT NULL PRIMARY KEY,"
               "CustomerType   VARCHAR(4),"
               "ExpirationDate VARCHAR(15),"
               "QtyBought      INTEGER DEFAULT 0,"
               "TotalSpent     DECIMAL(10,2) DEFAULT 0,"
               "TotalRebate    DECIMAL(10,2) DEFAULT 0,"
               "AnnualFee      DECIMAL(10,2) DEFAULT 0,"
               "ShouldUpgrade  VARCHAR(3));");

    query.exec("CREATE TABLE  SalesTable("
               "PurchaseDate  TEXT,"
               "CustomID      INTEGER NOT NULL,"
               "ItemName      VARCHAR(50),"
               "ItemPrice     DECIMAL(10,2) DEFAULT 0,"
               "Quantity      INTEGER DEFAULT 0 NOT NULL);");

    query.exec("CREATE TABLE  InventoryTable("
               "ItemName      VARCHAR(50),"
               "ItemPrice     DECIMAL(10,2),"
               "Quantity      INTEGER DEFAULT 0 NOT NULL,"
               "InStock       INTEGER DEFAULT 0 NOT NULL,"
               "Revenue       Decimal(10,2));");
}

/*!
 * \brief Reads the Shopper.txt file and inserts them into the CustomerTable
 */
void SQLDatabase::readFileCustomer()
{
    QFile file(":/Shoppers/warehouse shoppers.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream inFile(&file);

    if(file.isOpen())
    {
        qDebug() << "Opened File";
        while(!inFile.atEnd())
        {
            customerData.name = inFile.readLine();
            customerData.customerID = inFile.readLine();
            customerData.executiveType = inFile.readLine();
            customerData.expDate = inFile.readLine();

            // This function populates customerTable so dont uncomment it unless your table is empty.
            addCustomerIntoTable(customerData);
        }

        file.close();
    }

    else
        qDebug() << "Cannot open file thats used to Read File from Customer list";
}

/*!
 * \brief Reads the days.txt files and inserts them into the Sales table
 */
void SQLDatabase::readFileSales()
{

    std::string day = "day";
    std::string txt = ".txt";
    for (int i = 1; i <= 7; ++i) {
        if (day[3])
            day = "day";
        day = day + std::to_string(i) + txt;
        QString qstrDay = ":/Days/" + QString::fromStdString(day);
        QFile file(qstrDay);
        file.open(QIODevice::ReadOnly);
        QTextStream inFile(&file);

        if(file.isOpen())
        {
            qDebug() << "Opened File";
            while(!inFile.atEnd())
            {
                salesData.purchaseDate  = inFile.readLine();
                salesData.customerID    = inFile.readLine();
                salesData.itemName      = inFile.readLine();
                salesData.itemPrice     = inFile.readLine();
                salesData.quantity      = inFile.readLine();
                // Don't uncomment unless your table is empty
                addSalesIntoTable(salesData);

                qDebug() << "purchase date" << salesData.purchaseDate;
                qDebug() << "customerID   " << salesData.customerID;
                qDebug() << "itemName     " << salesData.itemName;
                qDebug() << "itemPrice    " << salesData.itemPrice;
                qDebug() << "quantity     " << salesData.quantity << Qt::endl << Qt::endl;


            }
            file.close();
            qDebug() << "day file: " << qstrDay << Qt::endl << Qt::endl;
        }

        else
            qDebug() << "Cannot open file that reads from the Sales list";
    }

}
/*!
 * \brief Returns the Database
 */
QSqlDatabase SQLDatabase::GetDatabase() const
{
    return database;
}

/*!
 * \brief Inserts the customers into the Customer Table
 */
void SQLDatabase::addCustomerIntoTable(customerTableInfo& customerData)
{
    QSqlQuery query;
    query.prepare("CREATE UNIQUE INDEX uidx_customerID"
                  "ON CustomerTable (CustomerID);");

    query.prepare("INSERT OR IGNORE INTO CustomerTable(Name, CustomerID, "
                  "CustomerType, ExpirationDate)"
                  "VALUES(:name, :ID, :Type, :ExpDate)");

    query.bindValue(":name", customerData.name);
    query.bindValue(":ID", customerData.customerID);
    query.bindValue(":Type", customerData.executiveType);
    query.bindValue(":ExpDate", customerData.expDate);


    if(!query.exec())
        qDebug() << "Failed: " << query.lastError();
}
/*!
 * \brief Inserts the sales info into the SalesTable
 */

void SQLDatabase::addSalesIntoTable(salesTableInfo& salesData)
{

    QSqlQuery query;

    query.prepare("INSERT INTO SalesTable(PurchaseDate, CustomID, ItemName, ItemPrice, Quantity)"
                  "VALUES(:date, :ID, :itemName, :itemPrice, :quantity)");

    query.bindValue(":date", salesData.purchaseDate);
    query.bindValue(":ID", salesData.customerID);
    query.bindValue(":itemName", salesData.itemName);
    query.bindValue(":itemPrice", salesData.itemPrice);
    query.bindValue(":quantity", salesData.quantity);

    if(!query.exec())
        qDebug() << "Failed: " << query.lastError();

    checkInventory();
}

/*!
 * \brief Inserts the Inventory into the InvenotryTable
 */
void SQLDatabase::handleInventory()
{
    QSqlQuery query;

    query.prepare("INSERT INTO InventoryTable(ItemName, ItemPrice, Quantity, InStock,Revenue)"
                  "VALUES(:name, :price, :quant, :stock,:rev)");
    query.bindValue(":name", inventoryData.itemName);
    query.bindValue(":price", inventoryData.itemPrice);
    query.bindValue(":quant", inventoryData.quantityPurchased);
    query.bindValue(":stock", inventoryData.inStock);
    query.bindValue(":rev", inventoryData.revenue);

    if(!query.exec()){
        qDebug() << "Failed: " << query.lastError();
    }
}
/*!
 * \brief Checks the InventoryTable and adds the Quantity if there is an existing item
 */
void SQLDatabase::checkInventory(){

    QSqlQuery query;
    query.prepare("SELECT * FROM InventoryTable"
                  " WHERE ItemName       LIKE '%" + salesData.itemName + "%'");

    query.bindValue(":searchingFor", salesData.itemName);

    if(!query.exec()) {
        qDebug() << query.lastError();
    }

    if (query.next()) {

        double itemPrice = query.value(1).toDouble();
        int quantFromDB = query.value(2).toInt();
        double totalRevenue = query.value(4).toDouble();
        int quantToInput = salesData.quantity.toInt();
        int newQuantForDb =  quantFromDB + quantToInput;
        int newStockForDb = 500 - newQuantForDb;
        totalRevenue = newQuantForDb * itemPrice;
        double dec = query.value(1).toString().toDouble();

       if(newStockForDb <= 0 || newStockForDb < 0){
            updateDB(0,quantFromDB,dec,query.value(4).toDouble());
       }
       else{
            updateDB(newStockForDb,newQuantForDb,dec,totalRevenue);
           }
       }
    else{
        inventoryData.itemName = salesData.itemName;
        inventoryData.itemPrice = salesData.itemPrice;
        inventoryData.quantityPurchased = salesData.quantity;
        int newStock = 500 - salesData.quantity.toInt();
        QString m;
        inventoryData.inStock = m.number(newStock);
        double totalRev = salesData.quantity.toInt() * inventoryData.itemPrice.toDouble();
        inventoryData.revenue = totalRev;
        handleInventory();
        }
}
/*!
 * \brief Updates the Inventory Table with the new or current Items
 */
void SQLDatabase::updateDB(int stock,int quant,double dec,double totalRevenue){
    QSqlQuery query;
    query.prepare("UPDATE InventoryTable "
                  "SET    ItemName = :name, "
                  "       ItemPrice = :price, "
                  "       Quantity = :quant, "
                  "       InStock = :stock, "
                  "       Revenue = :rev "
                  "WHERE  ItemName = :c;");

       QString price = price.number(dec,'f',2);
       QString rev = rev.number(totalRevenue,'f',2);
       qDebug() << price << " " << rev;

       query.bindValue(":name", salesData.itemName);
       query.bindValue(":price", price);
       query.bindValue(":quant", quant);
       query.bindValue(":stock", stock);
       query.bindValue(":rev", rev);
       query.bindValue(":c",salesData.itemName);

       if(!query.exec())
            qDebug() << query.lastError();
}
