#include "userwindow.h"
#include "ui_userwindow.h"

UserWindow::UserWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserWindow)
{
    ui->setupUi(this);

    setFixedSize(width(), height()); // 固定窗口大小
    setWindowTitle("书籍上架系统——用户操作界面"); // 设置标题
    setWindowIcon(QIcon(":/res/user.png")); // 设置图标

    // 设置初始页面
    ui->stackedWidget->setCurrentWidget(ui->booksPage);

    // 设置页面背景颜色
    ui->bookTableView->setStyleSheet("QTableView { background-color: rgb(225, 225, 225); }");
    ui->recordTableView->setStyleSheet("QTableView { background-color: rgb(225, 225, 225); }");
    ui->workTableView->setStyleSheet("QTableView { background-color: rgb(225, 225, 225); }");

    // 隐藏表格左侧的序号
    setupTableView(ui->bookTableView);
    setupTableView(ui->workTableView);
    setupTableView(ui->recordTableView);

    // 设置表格字体
    setupTableView2();

    // 连接书籍信息页面功能按钮
    connect(ui->showBooksButton, &QPushButton::clicked, this, &UserWindow::showBooksTable);
    connect(ui->showWorkButton, &QPushButton::clicked, this, &UserWindow::showWorkTable);
    connect(ui->showRecordsButton, &QPushButton::clicked, this, &UserWindow::showRecordsTable);

    // 连接图片展示功能按钮
    connect(ui->bookTableView, &QTableView::clicked, this, &UserWindow::onBookCellClicked);

    // 连接工作任务页面功能按钮
    connect(ui->addWorkButton, &QPushButton::clicked, this, &UserWindow::addWork);
    connect(ui->removeWorkButton, &QPushButton::clicked, this, &UserWindow::removeWork);
    connect(ui->modifyWorkButton, &QPushButton::clicked, this, &UserWindow::modifyWork);

    // 连接上书记录页面功能按钮
    connect(ui->addRecordButton, &QPushButton::clicked, this, &UserWindow::addRecord);
    connect(ui->removeRecordButton, &QPushButton::clicked, this, &UserWindow::removeRecord);
    connect(ui->modifyRecordButton, &QPushButton::clicked, this, &UserWindow::modifyRecord);

    // 连接退出及重新登陆功能按钮
    connect(ui->logoutButton, &QPushButton::clicked, this, &UserWindow::onLogoutButtonClicked);
    connect(ui->reloginButton, &QPushButton::clicked, this, &UserWindow::onReloginButtonClicked);
}

UserWindow::~UserWindow()
{
    delete ui;
}

void UserWindow::setupTableView(QTableView *tableView)
{
    // 隐藏表格左侧的序号
    tableView->verticalHeader()->setVisible(false);
}

void UserWindow::setupTableView2()
{
    QFont tableFont("Arial", 11); // 指定所需的字体和大小

    ui->bookTableView->setFont(tableFont);
    ui->workTableView->setFont(tableFont);
    ui->recordTableView->setFont(tableFont);
}

void UserWindow::setUserInfo(const QString &username, const QString &currentTime)
{
    this->username = username;
    this->currentTime = currentTime;
    timeUpdateTimer = new QTimer(this);
    timeUpdateTimer->setInterval(1000); // 更新间隔为1000毫秒，即1秒钟
    connect(timeUpdateTimer, &QTimer::timeout, this, &UserWindow::updateTime);
    timeUpdateTimer->start();

    ui->usernameLabel->setText("用户: " + username);
    ui->timeLabel->setText("当前时间: " + currentTime);
}

void UserWindow::updateTime() {
    // 获取当前时间
    QString newTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    // 更新时间显示
    ui->timeLabel->setText("当前时间: " + newTime);
}

int UserWindow::getUserIdFromUsername(const QString& username)
{
    // username 格式为"user" + 数字
    QRegExp rx("\\d+");
    if (rx.indexIn(username) != -1) {
        QString idStr = rx.cap(0);
        return idStr.toInt();
    }
    return -1; // 如果没有找到数字，返回错误ID
}

void UserWindow::showBooksTable()
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
    ui->workTableView->setModel(nullptr);
    ui->recordTableView->setModel(nullptr);
}

void UserWindow::showWorkTable()
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
    ui->recordTableView->setModel(nullptr);
}

void UserWindow::showRecordsTable()
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
    ui->workTableView->setModel(nullptr);
}

void UserWindow::onBookCellClicked(const QModelIndex &index)
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

// 工作任务页面槽函数
void UserWindow::addWork() {
    bool ok;

    int taskId = QInputDialog::getInt(this, "添加工作任务", "请输入任务ID:", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    int managerId = getUserIdFromUsername(username); // 使用当前用户的ID

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

void UserWindow::removeWork() {
    bool ok;

    int taskId = QInputDialog::getInt(this, "删除工作任务", "请输入要删除的任务ID:", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    int managerId = getUserIdFromUsername(username); // 使用当前用户的ID

    // 检查任务ID对应的负责人ID是否与当前用户ID相同
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT `负责人id` FROM `工作任务表` WHERE `任务id` = ?");
    checkQuery.addBindValue(taskId);

    if (!checkQuery.exec() || !checkQuery.next()) {
        QMessageBox::critical(this, "错误", "无法找到对应的任务或查询失败。");
        return;
    }

    int taskOwnerId = checkQuery.value(0).toInt(); // 获取任务的负责人ID
    if (taskOwnerId != managerId) {
        QMessageBox::critical(this, "错误", "您无权删除此任务。");
        return;
    }

    // 如果负责人ID相同，则执行删除操作
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

void UserWindow::modifyWork() {
    bool ok;

    int taskId = QInputDialog::getInt(this, "修改工作任务", "请输入要修改的任务ID:", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    int managerId = getUserIdFromUsername(username); // 使用当前用户的ID

    // 检查任务ID对应的负责人ID是否与当前用户ID相同
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT `负责人id` FROM `工作任务表` WHERE `任务id` = ?");
    checkQuery.addBindValue(taskId);

    if (!checkQuery.exec() || !checkQuery.next()) {
        QMessageBox::critical(this, "错误", "无法找到对应的任务或查询失败。");
        return;
    }

    int taskOwnerId = checkQuery.value(0).toInt(); // 获取任务的负责人ID
    if (taskOwnerId != managerId) {
        QMessageBox::critical(this, "错误", "您无权修改此任务。");
        return;
    }

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
    query.addBindValue(managerId); // 确保只有当前用户可以修改自己的任务

    if (!query.exec()) {
        QMessageBox::critical(this, "错误", "修改工作任务失败: " + query.lastError().text());
    } else {
        QMessageBox::information(this, "成功", "工作任务修改成功。");
    }

    showWorkTable();
}


// 上书记录页面槽函数
void UserWindow::addRecord() // 增加上书记录
{
    bool ok;
    int recordId = QInputDialog::getInt(this, "添加上书记录", "请输入记录ID:", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    int managerId = getUserIdFromUsername(username); // 使用当前用户的ID，不再从用户处获取

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

void UserWindow::removeRecord() // 删除上书记录
{
    bool ok;
    int recordId = QInputDialog::getInt(this, "删除上书记录", "请输入要删除的记录ID:", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    int managerId = getUserIdFromUsername(username); // 获取当前用户的ID

    // 检查记录ID对应的负责人ID是否与当前用户ID相同
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT `负责人id` FROM `上书记录` WHERE `记录ID` = ?");
    checkQuery.addBindValue(recordId);

    if (!checkQuery.exec() || !checkQuery.next()) {
        QMessageBox::critical(this, "错误", "无法找到对应的记录或查询失败。");
        return;
    }

    int recordOwnerId = checkQuery.value(0).toInt(); // 获取记录的负责人ID
    if (recordOwnerId != managerId) {
        QMessageBox::critical(this, "错误", "您无权删除此记录。");
        return;
    }

    // 如果负责人ID相同，则执行删除操作
    QSqlQuery deleteQuery;
    deleteQuery.prepare("DELETE FROM `上书记录` WHERE `记录ID` = ?");
    deleteQuery.addBindValue(recordId);

    if (!deleteQuery.exec()) {
        QMessageBox::critical(this, "错误", "删除上书记录失败: " + deleteQuery.lastError().text());
    } else {
        QMessageBox::information(this, "成功", "上书记录删除成功。");
    }

    showRecordsTable(); // 刷新记录表格
}

void UserWindow::modifyRecord() // 修改上书记录信息
{
    bool ok;
    int recordId = QInputDialog::getInt(this, "修改上书记录", "请输入要修改的记录ID:", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    int managerId = getUserIdFromUsername(username); // 使用当前用户的ID

    // 检查记录ID对应的负责人ID是否与当前用户ID相同
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT `负责人id` FROM `上书记录` WHERE `记录ID` = ?");
    checkQuery.addBindValue(recordId);

    if (!checkQuery.exec() || !checkQuery.next()) {
        QMessageBox::critical(this, "错误", "无法找到对应的记录或查询失败。");
        return;
    }

    int recordOwnerId = checkQuery.value(0).toInt(); // 获取记录的负责人ID
    if (recordOwnerId != managerId) {
        QMessageBox::critical(this, "错误", "您无权修改此记录。");
        return;
    }

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

void UserWindow::onLogoutButtonClicked()
{
    QApplication::quit(); // 退出应用程序
}

void UserWindow::onReloginButtonClicked()
{
    this->close(); // 关闭当前窗口
    login *loginWindow = new login();
    loginWindow->show(); // 显示登录窗口
}
