#include "adminwindow.h"
#include "ui_adminwindow.h"

AdminWindow::AdminWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminWindow)
{
    ui->setupUi(this);

    setFixedSize(width(), height()); // 固定窗口大小
    setWindowTitle("书籍上架系统——管理员操作界面"); // 设置标题
    setWindowIcon(QIcon(":/res/admin.png")); // 设置图标

    // 设置初始页面
    ui->stackedWidget->setCurrentWidget(ui->booksPage);

    // 设置页面背景颜色
    ui->bookTableView->setStyleSheet("QTableView { background-color: rgb(225, 225, 225); }");
    ui->userTableView->setStyleSheet("QTableView { background-color: rgb(225, 225, 225); }");
    ui->recordTableView->setStyleSheet("QTableView { background-color: rgb(225, 225, 225); }");
    ui->workTableView->setStyleSheet("QTableView { background-color: rgb(225, 225, 225); }");

    // 隐藏表格左侧的序号
    setupTableView(ui->bookTableView);
    setupTableView(ui->userTableView);
    setupTableView(ui->recordTableView);
    setupTableView(ui->workTableView);

    // 设置表格字体
    setupTableView2();

    // 连接按钮的点击信号到相应槽函数
    connect(ui->showBooksButton, &QPushButton::clicked, this, &AdminWindow::showBooksTable);
    connect(ui->showUsersButton, &QPushButton::clicked, this, &AdminWindow::showUsersTable);
    connect(ui->showRecordsButton, &QPushButton::clicked, this, &AdminWindow::showRecordsTable);
    connect(ui->showWorkButton, &QPushButton::clicked, this, &AdminWindow::showWorkTable);

    // 连接图片展示功能按钮
    connect(ui->bookTableView, &QTableView::clicked, this, &AdminWindow::onBookCellClicked);

    // 连接书籍信息页面功能按钮
    connect(ui->addBookButton, &QPushButton::clicked, this, &AdminWindow::addBook);
    connect(ui->removeBookButton, &QPushButton::clicked, this, &AdminWindow::removeBook);
    connect(ui->modifyBookButton, &QPushButton::clicked, this, &AdminWindow::modifyBook);

    // 连接用户页面功能按钮
    connect(ui->addUserButton, &QPushButton::clicked, this, &AdminWindow::addUser);
    connect(ui->removeUserButton, &QPushButton::clicked, this, &AdminWindow::removeUser);
    connect(ui->modifyUserButton, &QPushButton::clicked, this, &AdminWindow::modifyUser);

    // 连接上书记录页面功能按钮
    connect(ui->addRecordButton, &QPushButton::clicked, this, &AdminWindow::addRecord);
    connect(ui->removeRecordButton, &QPushButton::clicked, this, &AdminWindow::removeRecord);
    connect(ui->modifyRecordButton, &QPushButton::clicked, this, &AdminWindow::modifyRecord);

    // 连接工作任务页面功能按钮
    connect(ui->addWorkButton, &QPushButton::clicked, this, &AdminWindow::addWork);
    connect(ui->removeWorkButton, &QPushButton::clicked, this, &AdminWindow::removeWork);
    connect(ui->modifyWorkButton, &QPushButton::clicked, this, &AdminWindow::modifyWork);

    // 连接退出及重新登陆功能按钮
    connect(ui->logoutButton, &QPushButton::clicked, this, &AdminWindow::onLogoutButtonClicked);
    connect(ui->reloginButton, &QPushButton::clicked, this, &AdminWindow::onReloginButtonClicked);

    // 连接书籍信息查询页面，右键显示筛选、排序功能菜单
    ui->bookTableView->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->bookTableView->horizontalHeader(), &QHeaderView::customContextMenuRequested, this, &AdminWindow::showFilterMenu);
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

void AdminWindow::setupTableView(QTableView *tableView)
{
    // 隐藏表格左侧的序号
    tableView->verticalHeader()->setVisible(false);
}

void AdminWindow::setupTableView2()
{
    QFont tableFont("Arial", 11); // 指定所需的字体和大小

    ui->bookTableView->setFont(tableFont);
    ui->userTableView->setFont(tableFont);
    ui->recordTableView->setFont(tableFont);
}

void AdminWindow::setUserInfo(const QString &username, const QString &currentTime)
{
    this->username = username;
    this->currentTime = currentTime;
    timeUpdateTimer = new QTimer(this);
    timeUpdateTimer->setInterval(1000); // 更新间隔为1000毫秒，即1秒钟
    connect(timeUpdateTimer, &QTimer::timeout, this, &AdminWindow::updateTime);
    timeUpdateTimer->start();

    ui->usernameLabel->setText("用户: " + username);
    ui->timeLabel->setText("当前时间: " + currentTime);
}

void AdminWindow::updateTime() {
    // 获取当前时间
    QString newTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    // 更新时间显示
    ui->timeLabel->setText("当前时间: " + newTime);
}

void AdminWindow::showBooksTable()
{
    ui->stackedWidget->setCurrentWidget(ui->booksPage);

    // 创建模型
    QSqlQueryModel *queryModel = new QSqlQueryModel(this);

    // 设置查询
    queryModel->setQuery("SELECT * FROM 图书表");

    // 创建字体对象并设置字体大小为12
    QFont font("Arial", 12);
    font.setBold(true);
    ui->bookTableView->horizontalHeader()->setFont(font);

    // 设置表格标题颜色
    ui->bookTableView->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: rgb(209, 193, 172); }");

    // 设置表格标题
    queryModel->setHeaderData(0, Qt::Horizontal, "书籍序号");
    queryModel->setHeaderData(1, Qt::Horizontal, "书籍名称");
    queryModel->setHeaderData(2, Qt::Horizontal, "书籍作者");
    queryModel->setHeaderData(3, Qt::Horizontal, "书籍出版社");
    queryModel->setHeaderData(4, Qt::Horizontal, "书籍借还状态");
    queryModel->setHeaderData(5, Qt::Horizontal, "书籍数量");
    queryModel->setHeaderData(6, Qt::Horizontal, "书籍价格");
    queryModel->setHeaderData(7, Qt::Horizontal, "书籍类型1");
    queryModel->setHeaderData(8, Qt::Horizontal, "书籍类型2");
    queryModel->setHeaderData(9, Qt::Horizontal, "书籍类型3");
    queryModel->setHeaderData(10, Qt::Horizontal, "书籍ISBN码");
    queryModel->setHeaderData(11, Qt::Horizontal, "书籍图片");
    queryModel->setHeaderData(12, Qt::Horizontal, "书籍所在楼层");

    // 显示在相应 tableView 中
    ui->bookTableView->setModel(queryModel);
    ui->userTableView->setModel(nullptr);
    ui->recordTableView->setModel(nullptr);
    ui->workTableView->setModel(nullptr);
}

void AdminWindow::showUsersTable()
{
    ui->stackedWidget->setCurrentWidget(ui->usersPage);

    // 创建模型
    QSqlQueryModel *queryModel = new QSqlQueryModel(this);

    // 设置查询
    queryModel->setQuery("SELECT * FROM 用户表");

    // 创建字体对象并设置字体大小为12
    QFont font("Arial", 12);
    font.setBold(true);
    ui->userTableView->horizontalHeader()->setFont(font);

    // 设置表格标题颜色
    ui->userTableView->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: rgb(209, 193, 172); }");

    // 使标题栏自适应大小
    ui->userTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 设置表格标题
    queryModel->setHeaderData(0, Qt::Horizontal, "用户序号");
    queryModel->setHeaderData(1, Qt::Horizontal, "用户名");
    queryModel->setHeaderData(2, Qt::Horizontal, "用户密码");
    queryModel->setHeaderData(3, Qt::Horizontal, "用户类型");

    // 显示在相应 tableView 中
    ui->userTableView->setModel(queryModel);
    ui->bookTableView->setModel(nullptr);
    ui->recordTableView->setModel(nullptr);
    ui->workTableView->setModel(nullptr);
}

void AdminWindow::showRecordsTable()
{
    ui->stackedWidget->setCurrentWidget(ui->recordsPage);

    // 创建模型
    QSqlQueryModel *queryModel = new QSqlQueryModel(this);

    // 设置查询
    queryModel->setQuery("SELECT * FROM 上书记录");

    // 创建字体对象并设置字体大小为12
    QFont font("Arial", 12);
    font.setBold(true);
    ui->recordTableView->horizontalHeader()->setFont(font);

    // 设置表格标题颜色
    ui->recordTableView->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: rgb(209, 193, 172); }");

    // 使标题栏自适应大小
    ui->recordTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 设置表格标题
    queryModel->setHeaderData(0, Qt::Horizontal, "记录序号");
    queryModel->setHeaderData(1, Qt::Horizontal, "负责人序号");
    queryModel->setHeaderData(2, Qt::Horizontal, "书籍序号");
    queryModel->setHeaderData(3, Qt::Horizontal, "上书时间");
    queryModel->setHeaderData(4, Qt::Horizontal, "上书完成状态");

    // 显示在相应 tableView 中
    ui->recordTableView->setModel(queryModel);
    ui->bookTableView->setModel(nullptr);
    ui->userTableView->setModel(nullptr);
    ui->workTableView->setModel(nullptr);
}

void AdminWindow::showWorkTable()
{
    ui->stackedWidget->setCurrentWidget(ui->workPage);

    // 创建模型
    QSqlQueryModel *queryModel = new QSqlQueryModel(this);

    // 设置查询
    queryModel->setQuery("SELECT * FROM 工作任务表");

    // 创建字体对象并设置字体大小为12
    QFont font("Arial", 12);
    font.setBold(true);
    ui->workTableView->horizontalHeader()->setFont(font);

    // 设置表格标题颜色
    ui->workTableView->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: rgb(209, 193, 172); }");

    // 使标题栏自适应大小
    ui->workTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 设置表格标题
    queryModel->setHeaderData(0, Qt::Horizontal, "任务序号");
    queryModel->setHeaderData(1, Qt::Horizontal, "负责人序号");
    queryModel->setHeaderData(2, Qt::Horizontal, "书籍序号");
    queryModel->setHeaderData(3, Qt::Horizontal, "工作完成状态");

    // 显示在相应 tableView 中
    ui->workTableView->setModel(queryModel);
    ui->bookTableView->setModel(nullptr);
    ui->userTableView->setModel(nullptr);
    ui->recordTableView->setModel(nullptr);
}

void AdminWindow::onBookCellClicked(const QModelIndex &index)
{
    if (index.column() == 11) // 第 11 列是图片列
    {
        QSqlQuery query;
        query.prepare("SELECT pic FROM 图书表 WHERE bookid = ?");
        query.addBindValue(index.sibling(index.row(), 0).data()); // 书籍序号
        query.exec();

        if (query.next()) {
            QString imagePath = query.value(0).toString();

            QPixmap pixmap;
            pixmap.load(imagePath);

            ImageDialog *dialog = new ImageDialog(pixmap, this);
            dialog->exec();
        } else {
            QMessageBox::warning(this, "Warning", "该书籍暂未上传图片.");
        }

    }
}

// 书籍页面槽函数
void AdminWindow::addBook() // 增加图书
{
    bool ok;
    int bookId = QInputDialog::getInt(this, "添加图书", "请输入图书ID:", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    QString name = QInputDialog::getText(this, "添加图书", "请输入图书名称:", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QString author = QInputDialog::getText(this, "添加图书", "请输入作者名称:", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QString pub = QInputDialog::getText(this, "添加图书", "请输入出版社:", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QString status = QInputDialog::getText(this, "添加图书", "请输入状态（如'在馆'）:", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    int num = QInputDialog::getInt(this, "添加图书", "请输入数量:", 0, 0, 1000, 1, &ok);
    if (!ok) return;

    double price = QInputDialog::getDouble(this, "添加图书", "请输入价格:", 0.0, 0.0, 10000.0, 2, &ok);
    if (!ok) return;

    QString type1 = QInputDialog::getText(this, "添加图书", "请输入类别1:", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QString type2 = QInputDialog::getText(this, "添加图书", "请输入类别2:", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QString type3 = QInputDialog::getText(this, "添加图书", "请输入类别3:", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QString code = QInputDialog::getText(this, "添加图书", "请输入ISBN码:", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QString pic = QFileDialog::getOpenFileName(this, "选择图书图片", "", "图片 (*.png *.jpg *.bmp)", nullptr, QFileDialog::DontUseNativeDialog);
    if (pic.isEmpty()) pic = "default_image_path"; // 可以设置默认图片路径

    int floor = QInputDialog::getInt(this, "添加图书", "请输入楼层号:", 1, 1, 10, 1, &ok);
    if (!ok) return;

    QSqlQuery query;
    query.prepare("INSERT INTO `图书表` (`bookid`, `name`, `auther`, `pub`, `status`, `num`, `price`, `type1`, `type2`, `type3`, `code`, `pic`, `floor`) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(bookId);
    query.addBindValue(name);
    query.addBindValue(author);
    query.addBindValue(pub);
    query.addBindValue(status);
    query.addBindValue(num);
    query.addBindValue(price);
    query.addBindValue(type1);
    query.addBindValue(type2);
    query.addBindValue(type3);
    query.addBindValue(code);
    query.addBindValue(pic);
    query.addBindValue(floor);

    if (!query.exec()) {
        QMessageBox::critical(this, "错误", "添加图书失败: " + query.lastError().text());
    } else {
        QMessageBox::information(this, "成功", "图书添加成功。");
    }

    showBooksTable(); // 刷新表格
}

void AdminWindow::removeBook() // 删除图书
{
    bool ok;
    int bookId = QInputDialog::getInt(this, "删除图书", "请输入要删除的图书ID:", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    QSqlQuery query;
    query.prepare("DELETE FROM `图书表` WHERE `bookid` = ?");
    query.addBindValue(bookId);

    if (!query.exec()) {
        QMessageBox::critical(this, "错误", "删除图书失败: " + query.lastError().text());
    } else {
        QMessageBox::information(this, "成功", "图书删除成功。");
    }

    showBooksTable(); // 刷新表格
}

void AdminWindow::modifyBook() // 修改图书信息
{
    bool ok;
    int bookId = QInputDialog::getInt(this, "修改图书", "请输入要修改的图书ID:", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    QString name = QInputDialog::getText(this, "修改图书", "请输入新图书名称:", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QString author = QInputDialog::getText(this, "修改图书", "请输入新作者名称:", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QString pub = QInputDialog::getText(this, "修改图书", "请输入新出版社:", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QString status = QInputDialog::getText(this, "修改图书", "请输入新状态（如'在馆'）:", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    int num = QInputDialog::getInt(this, "修改图书", "请输入新数量:", 0, 0, 1000, 1, &ok);
    if (!ok) return;

    double price = QInputDialog::getDouble(this, "修改图书", "请输入新价格:", 0.0, 0.0, 10000.0, 2, &ok);
    if (!ok) return;

    QString type1 = QInputDialog::getText(this, "修改图书", "请输入新类别1:", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QString type2 = QInputDialog::getText(this, "修改图书", "请输入新类别2:", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QString type3 = QInputDialog::getText(this, "修改图书", "请输入新类别3:", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QString code = QInputDialog::getText(this, "修改图书", "请输入新ISBN码:", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QString pic = QFileDialog::getOpenFileName(this, "选择新图书图片", "", "图片 (*.png *.jpg *.bmp)", nullptr, QFileDialog::DontUseNativeDialog);
    if (pic.isEmpty()) pic = "default_image_path"; // 可以设置默认图片路径

    int floor = QInputDialog::getInt(this, "修改图书", "请输入新楼层号:", 1, 1, 10, 1, &ok);
    if (!ok) return;

    QSqlQuery query;
    query.prepare("UPDATE `图书表` SET `name` = ?, `auther` = ?, `pub` = ?, `status` = ?, `num` = ?, `price` = ?, `type1` = ?, `type2` = ?, `type3` = ?, `code` = ?, `pic` = ?, `floor` = ? WHERE `bookid` = ?");
    query.addBindValue(name);
    query.addBindValue(author);
    query.addBindValue(pub);
    query.addBindValue(status);
    query.addBindValue(num);
    query.addBindValue(price);
    query.addBindValue(type1);
    query.addBindValue(type2);
    query.addBindValue(type3);
    query.addBindValue(code);
    query.addBindValue(pic);
    query.addBindValue(floor);
    query.addBindValue(bookId);

    if (!query.exec()) {
        QMessageBox::critical(this, "错误", "修改图书失败: " + query.lastError().text());
    } else {
        QMessageBox::information(this, "成功", "图书修改成功。");
    }

    showBooksTable(); // 刷新表格
}


// 用户页面槽函数
void AdminWindow::addUser() // 增加用户
{
    bool ok;
    int userId = QInputDialog::getInt(this, "添加用户", "请输入用户ID:", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    QString nickname = QInputDialog::getText(this, "添加用户", "请输入昵称:", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QString password = QInputDialog::getText(this, "添加用户", "请输入密码:", QLineEdit::Password, "", &ok);
    if (!ok) return;

    QString type = QInputDialog::getText(this, "添加用户", "请输入用户类型（如'admin'或'user'）:", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QSqlQuery query;
    query.prepare("INSERT INTO `用户表` (`id`, `nickname`, `password`, `type`) VALUES (?, ?, ?, ?)");
    query.addBindValue(userId);
    query.addBindValue(nickname);
    query.addBindValue(password);
    query.addBindValue(type);

    if (!query.exec()) {
        QMessageBox::critical(this, "错误", "添加用户失败: " + query.lastError().text());
    } else {
        QMessageBox::information(this, "成功", "用户添加成功。");
    }

    showUsersTable(); // 刷新用户表格
}

void AdminWindow::removeUser() // 删除用户
{
    bool ok;
    int userId = QInputDialog::getInt(this, "删除用户", "请输入要删除的用户ID:", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    QSqlQuery query;
    query.prepare("DELETE FROM `用户表` WHERE `id` = ?");
    query.addBindValue(userId);

    if (!query.exec()) {
        QMessageBox::critical(this, "错误", "删除用户失败: " + query.lastError().text());
    } else {
        QMessageBox::information(this, "成功", "用户删除成功。");
    }

    showUsersTable(); // 刷新用户表格
}

void AdminWindow::modifyUser() // 修改用户信息
{
    bool ok;
    int userId = QInputDialog::getInt(this, "修改用户", "请输入要修改的用户ID:", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    QString nickname = QInputDialog::getText(this, "修改用户", "请输入新昵称:", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QString password = QInputDialog::getText(this, "修改用户", "请输入新密码:", QLineEdit::Password, "", &ok);
    if (!ok) return;

    QString type = QInputDialog::getText(this, "修改用户", "请输入新用户类型（如'admin'或'user'）:", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QSqlQuery query;
    query.prepare("UPDATE `用户表` SET `nickname` = ?, `password` = ?, `type` = ? WHERE `id` = ?");
    query.addBindValue(nickname);
    query.addBindValue(password);
    query.addBindValue(type);
    query.addBindValue(userId);

    if (!query.exec()) {
        QMessageBox::critical(this, "错误", "修改用户失败: " + query.lastError().text());
    } else {
        QMessageBox::information(this, "成功", "用户修改成功。");
    }

    showUsersTable(); // 刷新用户表格
}


// 上书记录页面槽函数
void AdminWindow::addRecord() // 增加上书记录
{
    bool ok;
    int recordId = QInputDialog::getInt(this, "添加上书记录", "请输入记录ID:", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    int managerId = QInputDialog::getInt(this, "添加上书记录", "请输入负责人ID:", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    int bookId = QInputDialog::getInt(this, "添加上书记录", "请输入图书ID:", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    QString time = QInputDialog::getText(this, "添加上书记录", "请输入上书时间 (格式: yyyy-MM-dd HH:mm:ss):", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QString status = QInputDialog::getText(this, "添加上书记录", "请输入上书状态 (如'done'或'not yet'):", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QSqlQuery query;
    query.prepare("INSERT INTO `上书记录` (`记录ID`, `负责人id`, `图书id`, `time`, `status`) VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(recordId);
    query.addBindValue(managerId);
    query.addBindValue(bookId);
    query.addBindValue(time);
    query.addBindValue(status);

    if (!query.exec()) {
        QMessageBox::critical(this, "错误", "添加上书记录失败: " + query.lastError().text());
        return;
    }

    // 根据状态更新图书状态
    QString bookStatus = (status == "done") ? "在馆" : "待上书";
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE `图书表` SET `status` = ? WHERE `bookid` = ?");
    updateQuery.addBindValue(bookStatus);
    updateQuery.addBindValue(bookId);

    if (!updateQuery.exec()) {
        QMessageBox::critical(this, "错误", "更新图书状态失败: " + updateQuery.lastError().text());
    } else {
        QMessageBox::information(this, "成功", "上书记录添加成功。");
    }

    showRecordsTable(); // 刷新记录表格
    showBooksTable();   // 刷新图书表格
}

void AdminWindow::removeRecord() // 删除上书记录
{
    bool ok;
    int recordId = QInputDialog::getInt(this, "删除上书记录", "请输入要删除的记录ID:", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    QSqlQuery query;
    query.prepare("DELETE FROM `上书记录` WHERE `记录ID` = ?"); // 确保表中有 `记录ID` 列
    query.addBindValue(recordId);

    if (!query.exec()) {
        QMessageBox::critical(this, "错误", "删除上书记录失败: " + query.lastError().text());
    } else {
        QMessageBox::information(this, "成功", "上书记录删除成功。");
    }

    showRecordsTable(); // 刷新记录表格
}

void AdminWindow::modifyRecord() // 修改上书记录信息
{
    bool ok;
    int recordId = QInputDialog::getInt(this, "修改上书记录", "请输入要修改的记录ID:", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    int managerId = QInputDialog::getInt(this, "修改上书记录", "请输入新负责人ID:", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    int bookId = QInputDialog::getInt(this, "修改上书记录", "请输入新图书ID:", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    QString time = QInputDialog::getText(this, "修改上书记录", "请输入新上书时间 (格式: yyyy-MM-dd HH:mm:ss):", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QString status = QInputDialog::getText(this, "修改上书记录", "请输入新上书状态 (如'done'或'not yet'):", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    // 先获取旧记录的状态
    QSqlQuery oldStatusQuery;
    oldStatusQuery.prepare("SELECT `status` FROM `上书记录` WHERE `记录ID` = ?");
    oldStatusQuery.addBindValue(recordId);
    if (!oldStatusQuery.exec() || !oldStatusQuery.next()) {
        QMessageBox::critical(this, "错误", "获取旧记录状态失败: " + oldStatusQuery.lastError().text());
        return;
    }
    QString oldStatus = oldStatusQuery.value(0).toString();

    // 更新记录
    QSqlQuery query;
    query.prepare("UPDATE `上书记录` SET `负责人id` = ?, `图书id` = ?, `time` = ?, `status` = ? WHERE `记录ID` = ?");
    query.addBindValue(managerId);
    query.addBindValue(bookId);
    query.addBindValue(time);
    query.addBindValue(status);
    query.addBindValue(recordId);

    if (!query.exec()) {
        QMessageBox::critical(this, "错误", "修改上书记录失败: " + query.lastError().text());
        return;
    }

    // 根据新状态更新图书状态
    QString bookStatus = (status == "done") ? "在馆" : "待上书";
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE `图书表` SET `status` = ? WHERE `bookid` = ?");
    updateQuery.addBindValue(bookStatus);
    updateQuery.addBindValue(bookId);

    if (!updateQuery.exec()) {
        QMessageBox::critical(this, "错误", "更新图书状态失败: " + updateQuery.lastError().text());
    } else {
        QMessageBox::information(this, "成功", "上书记录修改成功。");
    }

    showRecordsTable(); // 刷新记录表格
    showBooksTable();   // 刷新图书表格
}

// 工作任务页面槽函数
void AdminWindow::addWork() {
    bool ok;

    int taskId = QInputDialog::getInt(this, "添加工作任务", "请输入任务ID:", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    int managerId = QInputDialog::getInt(this, "修改上书记录", "请输入新负责人ID:", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    int bookId = QInputDialog::getInt(this, "添加工作任务", "请输入图书ID:", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    QString workStatus = QInputDialog::getText(this, "添加工作任务", "请输入工作完成状态 (如'done'或'not yet'):", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QSqlQuery query;
    query.prepare("INSERT INTO `工作任务表` (`任务id`, `负责人id`, `图书id`, `工作完成状态`) VALUES (?, ?, ?, ?)");
    query.addBindValue(taskId);
    query.addBindValue(managerId);
    query.addBindValue(bookId);
    query.addBindValue(workStatus);

    if (!query.exec()) {
        QMessageBox::critical(this, "错误", "添加工作任务失败: " + query.lastError().text());
    } else {
        QMessageBox::information(this, "成功", "工作任务添加成功。");
    }

    showWorkTable();
}

void AdminWindow::removeWork() {
    bool ok;

    int taskId = QInputDialog::getInt(this, "删除工作任务", "请输入要删除的任务ID:", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    // 执行删除操作
    QSqlQuery deleteQuery;
    deleteQuery.prepare("DELETE FROM `工作任务表` WHERE `任务id` = ?");
    deleteQuery.addBindValue(taskId);

    if (!deleteQuery.exec()) {
        QMessageBox::critical(this, "错误", "删除工作任务失败: " + deleteQuery.lastError().text());
    } else {
        QMessageBox::information(this, "成功", "工作任务删除成功。");
    }

    showWorkTable();
}

void AdminWindow::modifyWork() {
    bool ok;
    int taskId = QInputDialog::getInt(this, "修改工作任务", "请输入要修改的任务ID:", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    int newManagerId = QInputDialog::getInt(this, "修改工作任务", "请输入新负责人ID:", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    int bookId = QInputDialog::getInt(this, "修改工作任务", "请输入新的图书ID:", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    QString workStatus = QInputDialog::getText(this, "修改工作任务", "请输入新的工作完成状态 (如'done'或'not yet'):", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    // 更新任务
    QSqlQuery query;
    query.prepare("UPDATE `工作任务表` SET `图书id` = ?, `工作完成状态` = ? WHERE `任务id` = ? AND `负责人id` = ?");
    query.addBindValue(bookId);
    query.addBindValue(workStatus);
    query.addBindValue(taskId);
    query.addBindValue(newManagerId); // 允许管理员修改任何用户的ID

    if (!query.exec()) {
        QMessageBox::critical(this, "错误", "修改工作任务失败: " + query.lastError().text());
    } else {
        QMessageBox::information(this, "成功", "工作任务修改成功。");
    }

    showWorkTable();
}

void AdminWindow::onLogoutButtonClicked()
{
    QApplication::quit(); // 退出应用程序
}

void AdminWindow::onReloginButtonClicked()
{
    this->close(); // 关闭当前窗口
    login *loginWindow = new login();
    loginWindow->show(); // 显示登录窗口
}

void AdminWindow::showFilterMenu(const QPoint &pos) // 右键显示功能菜单
{
    int logicalIndex = ui->bookTableView->horizontalHeader()->logicalIndexAt(pos);

    if (logicalIndex != 2 && logicalIndex != 4 && logicalIndex != 7 && logicalIndex != 12 && logicalIndex != 6)
    {
        // 对于不需要筛选或排序的列，不显示右键菜单
        return;
    }

    QMenu menu;

    if (logicalIndex == 6) // 书籍价格列
    {
        QAction *sortAscAction = new QAction("升序排序", this);
        QAction *sortDescAction = new QAction("降序排序", this);

        connect(sortAscAction, &QAction::triggered, this, [this]() {
            sortBooksByPrice(true);  // 升序
        });

        connect(sortDescAction, &QAction::triggered, this, [this]() {
            sortBooksByPrice(false); // 降序
        });

        menu.addAction(sortAscAction);
        menu.addAction(sortDescAction);
    }
    else
    {
        QAction *filterAction = new QAction("筛选", this);
        connect(filterAction, &QAction::triggered, this, [this, logicalIndex]() {
            switch (logicalIndex)
            {
            case 2: // 书籍作者列
                filterBooksByAuthor();
                break;
            case 4: // 书籍借还状态列
                filterBooksByStatus();
                break;
            case 7: // 书籍类型1列
                filterBooksByType1();
                break;
            case 12: // 书籍所在楼层列
                filterBooksByFloor();
                break;
            default:
                break;
            }
        });

        menu.addAction(filterAction);
    }

    menu.exec(ui->bookTableView->viewport()->mapToGlobal(pos));
}

void AdminWindow::filterBooksByAuthor() // 根据书籍作者筛选书籍信息
{
    bool ok;
    QString filterText = QInputDialog::getText(this, "筛选", "请输入书籍作者的关键字：", QLineEdit::Normal, "", &ok);
    if (ok && !filterText.isEmpty())
    {
        QSqlQuery *query = new QSqlQuery(QSqlDatabase::database());
        QString queryStr = QString("SELECT * FROM 图书表 WHERE author LIKE '%%1%'").arg(filterText);

        if (!query->exec(queryStr))
        {
            qWarning() << "Query execution failed:" << query->lastError().text();
            return;
        }

        QSqlQueryModel *queryModel = new QSqlQueryModel(this);
        queryModel->setQuery(*query);

        // 更新表格显示
        updateTableView(queryModel);
    }
}

void AdminWindow::filterBooksByStatus() // 根据书籍状态信息筛选书籍信息
{
    bool ok;
    QString filterText = QInputDialog::getText(this, "筛选", "请输入书籍状态信息的关键字：", QLineEdit::Normal, "", &ok);
    if (ok && !filterText.isEmpty())
    {
        QSqlQuery *query = new QSqlQuery(QSqlDatabase::database());
        QString queryStr = QString("SELECT * FROM 图书表 WHERE status LIKE '%%1%'").arg(filterText);

        if (!query->exec(queryStr))
        {
            qWarning() << "Query execution failed:" << query->lastError().text();
            return;
        }

        QSqlQueryModel *queryModel = new QSqlQueryModel(this);
        queryModel->setQuery(*query);

        // 更新表格显示
        updateTableView(queryModel);
    }
}

void AdminWindow::filterBooksByType1() // 根据书籍类型1筛选书籍信息
{
    bool ok;
    QString filterText = QInputDialog::getText(this, "筛选", "请输入书籍类型1的关键字：", QLineEdit::Normal, "", &ok);
    if (ok && !filterText.isEmpty())
    {
        QSqlQuery *query = new QSqlQuery(QSqlDatabase::database());
        QString queryStr = QString("SELECT * FROM 图书表 WHERE type1 LIKE '%%1%'").arg(filterText);

        if (!query->exec(queryStr))
        {
            qWarning() << "Query execution failed:" << query->lastError().text();
            return;
        }

        QSqlQueryModel *queryModel = new QSqlQueryModel(this);
        queryModel->setQuery(*query);

        // 更新表格显示
        updateTableView(queryModel);
    }
}

void AdminWindow::filterBooksByFloor() // 根据书籍所在楼层筛选书籍信息
{
    bool ok;
    QString filterText = QInputDialog::getText(this, "筛选", "请输入书籍所在楼层的关键字：", QLineEdit::Normal, "", &ok);
    if (ok && !filterText.isEmpty())
    {
        QSqlQuery *query = new QSqlQuery(QSqlDatabase::database());
        QString queryStr = QString("SELECT * FROM 图书表 WHERE floor LIKE '%%1%'").arg(filterText);

        if (!query->exec(queryStr))
        {
            qWarning() << "Query execution failed:" << query->lastError().text();
            return;
        }

        QSqlQueryModel *queryModel = new QSqlQueryModel(this);
        queryModel->setQuery(*query);

        // 更新表格显示
        updateTableView(queryModel);
    }
}

void AdminWindow::sortBooksByPrice(bool ascending) // 实现书籍按价格排序功能
{
    QSqlQueryModel *queryModel = new QSqlQueryModel(this);
    QString order = ascending ? "ASC" : "DESC";
    QString queryStr = QString("SELECT * FROM 图书表 ORDER BY price %1").arg(order);

    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database());
    if (!query->exec(queryStr))
    {
        qWarning() << "Query execution failed:" << query->lastError().text();
        return;
    }

    queryModel->setQuery(*query);

    // 更新表格显示
    updateTableView(queryModel);
}

void AdminWindow::updateTableView(QSqlQueryModel *queryModel) // 更新表格显示
{
    // 设置表格标题字体大小
    QFont font("Arial", 12);
    font.setBold(true);
    ui->bookTableView->horizontalHeader()->setFont(font);
    ui->bookTableView->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: rgb(209, 193, 172); }");

    // 设置表格标题
    queryModel->setHeaderData(0, Qt::Horizontal, "书籍序号");
    queryModel->setHeaderData(1, Qt::Horizontal, "书籍名称");
    queryModel->setHeaderData(2, Qt::Horizontal, "书籍作者");
    queryModel->setHeaderData(3, Qt::Horizontal, "书籍出版社");
    queryModel->setHeaderData(4, Qt::Horizontal, "书籍借还状态");
    queryModel->setHeaderData(5, Qt::Horizontal, "书籍数量");
    queryModel->setHeaderData(6, Qt::Horizontal, "书籍价格");
    queryModel->setHeaderData(7, Qt::Horizontal, "书籍类型1");
    queryModel->setHeaderData(8, Qt::Horizontal, "书籍类型2");
    queryModel->setHeaderData(9, Qt::Horizontal, "书籍类型3");
    queryModel->setHeaderData(10, Qt::Horizontal, "书籍ISBN码");
    queryModel->setHeaderData(11, Qt::Horizontal, "书籍图片");
    queryModel->setHeaderData(12, Qt::Horizontal, "书籍所在楼层");

    // 显示在 tableView 中
    ui->bookTableView->setModel(queryModel);
}
