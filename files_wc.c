#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define HALL_FILE_PATH "halls.txt"//魏澄

// 定义结构体
struct Administrator {
    char adminID[11];    //帅哥管理员ID，长度6-10字符
    char cinemaName[51]; // 所属美女影院名称
    char name[51];       // 美女管理员姓名
    char email[51];      // meinv管理员邮箱
    char password[51];   // meinv管理员密码
};

struct User {
    char userID[11]; // 用户ID，长度6-10字符
    char name[51];   // 用户姓名
    char gender;     // 性别
    char phone[12];  // 电话号码，11位数字
    char password[51];// 用户密码
    char email[51];  // 用户邮箱
    float balance;   // 账户余额
};

struct MovieSession {
    char sessionID[11]; // 场次号
    char movieName[51];// 影片名
    char cinemaName[51];// 所属影院
    int hallID;         // 放映影厅ID
    char startTime[6];  // 电影开始时间，格式hh:mm
    char endTime[6];    // 电影结束时间
    int duration;       // 电影时长，单位分钟
    int remainingSeats;// 场次余票数
    float ticketPrice;  // 场次票价
    char language[11]; // 放映类型，如英语、中文等
    // 可能还有其他信息，如优惠活动等
};

struct Hall {
    int hallID;           // 影厅编号
    int totalSeats;       // 影厅总座位数
    // 影厅座位分布图和座位编号等信息可以根据需要添加
    char hallType[11];    // 影厅类型，如3D、4D、imax等
};

//全局变量
int Uresult;                     //chw用户序号的全局变量
int Aresult;                     //chw管理员序号的全局变量
int sessionCount = 20;            //chw当前场次数量,初始化了20个，从20开始
int hallCount = 5;               // 魏澄 当前影厅数量
//结构体数组
struct User users[100];          // 用户数组
struct MovieSession sessions[100];// 电影场次数组
struct Hall halls[100];            // 影厅数组
struct Administrator admins[5];
// 函数声明
void saveHallsToFile();//魏澄
void saveDataToFile();//魏澄
void loadHallsFromFile();//魏澄
void loadDataFromFile();//魏澄
void initializeAdministrators(struct Administrator admins[], int size);
void initializeUsers(struct User users[], int size);
void initializeHalls(struct Hall halls[], int size);
void initializeMovieSessions(struct MovieSession sessions[], int size);
int userLogin();  //返回用户信息
int adminLogin();
void displayUserInfo(int a);
void modifyUserInfo(int a);
void searchMovie();
void displayOrder();
void manageOrder();
void addMovie();
void showMovies();
void manageHall(struct Hall halls[], int *hallCount);//管理影厅
void showHalls(const struct Hall halls[], int size);//展示所有影厅信息
void addHall(struct Hall halls[], int *size);//添加影厅
void showHallInfo(const struct Hall *hall);//展示指定影厅信息
void manageSessions(struct MovieSession sessions[], int *sessionCount);    //场次管理函数
void showSessions();
void managePersonalInfo();
void querySessionsByAdminID(const char* adminID, struct Administrator admins[], int adminCount, struct MovieSession sessions[], int *sessionCount); //找到管理员相应的影院
void queryTodaySessions(struct MovieSession sessions[], int count, const char* input);//查询场次函数
void clearInputBuffer();//清空缓冲区
void addSession(struct MovieSession sessions[], int *sessionSize);//增加场次的函数






int main() {

    // 从文件加载数据 魏澄
    loadDataFromFile();

    int userType;
    // 初始化数据
    initializeUsers(users, 100);
    initializeAdministrators(admins, 5);
   // 去掉 initializeHalls 函数调用 魏澄 initializeHalls(halls, 5);
    initializeMovieSessions(sessions, 20);
    //初始化空ID
    for(int i=20; i<100; i++){
        strcpy(sessions[i].sessionID, "");
    }
    while (1) {
        printf("请选择登录类型:1.用户 2.管理员\n");
        scanf("%d", &userType);

        if (userType == 1) {
            Uresult = userLogin(); // 用户登录
            int userChoice;

            do {
                printf("\n用户功能:\n");
                printf("1. 查询影片\n2. 显示订单\n3. 查看个人信息\n4. 修改个人信息\n5. 注销\n");
                scanf("%d", &userChoice);

                switch (userChoice) {
                    case 1:
                        searchMovie(); // 查询影片
                        char input[102];
                        clearInputBuffer();
                        fgets(input, sizeof(input), stdin);
                        input[strcspn(input, "\n")] = 0;
                        queryTodaySessions(sessions, 20, input); 
                        break;
                    case 2:
                        displayOrder(); // 显示订单
                        break;
                    case 3:
                        displayUserInfo(Uresult); // 查看个人信息
                        break;
                    case 4:
                        modifyUserInfo(Uresult); // 修改个人信息
                        break;
                    case 5:
                        printf("注销中...\n");
                        break;
                    default:
                        printf("无效选择，请重试。\n");
                }
            } while (userChoice != 5);
            break; // 用户操作完成后退出循环
        } 
        
        
        else if (userType == 2) {
            Aresult = adminLogin(); // 管理员登录
            int adminChoice;

            do {
                printf("\n管理员功能:\n");
                printf("1. 订单管理\n2. 影厅管理\n3. 场次管理\n4. 个人信息管理\n5. 注销\n");
                scanf("%d", &adminChoice);

                switch (adminChoice) {
                    case 1:
                        manageOrder(); // 订单管理
                        break;
                    case 2:
                        manageHall(halls, &hallCount); // 影厅管理
                        break;
                    case 3:
                        manageSessions(sessions, &sessionCount); // 场次管理
                        break;
                    case 4:
                        managePersonalInfo(); // 个人信息管理
                        break;
                    case 5:
                        printf("注销中...\n");
                        break;
                    default:
                        printf("无效选择，请重试。\n");
                }
            } while (adminChoice != 5);
            break; // 管理员操作完成后退出循环
        } else {
            printf("无效选择，请重试。\n");
        }
    }

     // 保存数据到文件 魏澄
    saveDataToFile();

    return 0;
}

//存储影厅  魏澄
void saveHallsToFile() {
    FILE *file = fopen(HALL_FILE_PATH, "w");
    if (file == NULL) {
        printf("无法打开影厅文件进行写入。\n");
        return;
    }
    for (int i = 0; i < hallCount; i++) {
        fprintf(file, "%d %d %s\n", halls[i].hallID, halls[i].totalSeats, halls[i].hallType);
    }
    fclose(file);
}

//存储函数的封装 魏澄
void saveDataToFile() {
    //saveAdminsToFile();
    //saveUsersToFile();
    //saveSessionsToFile();
    saveHallsToFile();
}

//读取影厅 魏澄
void loadHallsFromFile() {
    FILE *file = fopen(HALL_FILE_PATH, "r");
    if (file == NULL) {
        printf("无法打开影厅文件进行读取。\n");
        return;
    }
    hallCount = 0;
    while (fscanf(file, "%d %d %s", &halls[hallCount].hallID, &halls[hallCount].totalSeats, halls[hallCount].hallType) != EOF) {
        hallCount++;
    }
    fclose(file);
}

//读取函数的封装 魏澄
void loadDataFromFile() {
   // loadAdminsFromFile();
   // loadUsersFromFile();
   // loadSessionsFromFile();
    loadHallsFromFile();
}

// 初始化管理员数组的函数
void initializeAdministrators(struct Administrator admins[], int size) {
    for (int i = 0; i < size; i++) {
        snprintf(admins[i].adminID, sizeof(admins[i].adminID), "ADM%06d", i);
        snprintf(admins[i].cinemaName, sizeof(admins[i].cinemaName), "Cinema%d", i);
        snprintf(admins[i].name, sizeof(admins[i].name), "AdminName%d", i);
        snprintf(admins[i].email, sizeof(admins[i].email), "admin%d@example.com", i);
        snprintf(admins[i].password, sizeof(admins[i].password), "password%d", i);
    }
}

// 初始化用户数组的函数
void initializeUsers(struct User users[], int size) {
    for (int i = 0; i < size; i++) {
        snprintf(users[i].userID, sizeof(users[i].userID), "U%06d", i);
        snprintf(users[i].name, sizeof(users[i].name), "UserName%d", i);
        users[i].gender = 'M'; // 假设所有用户都是男性
        snprintf(users[i].phone, sizeof(users[i].phone), "13800138%02d", i);
        snprintf(users[i].password, sizeof(users[i].password), "password%d", i);
        snprintf(users[i].email, sizeof(users[i].email), "user%d@example.com", i);
        users[i].balance = 100.0f; // 所有用户账户余额初始化为100.0元
    }
}

// 初始化影厅数组的函数
void initializeHalls(struct Hall halls[], int size) {
    for (int i = 0; i < size; i++) {
        halls[i].hallID = i + 1;
        halls[i].totalSeats = 100 + i * 50; // 假设座位数递增
        snprintf(halls[i].hallType, sizeof(halls[i].hallType), "Type%d", i);
    }
}

// 初始化场次数组的函数
void initializeMovieSessions(struct MovieSession sessions[], int size) {
    for (int i = 0; i < size; i++) {
        snprintf(sessions[i].sessionID, sizeof(sessions[i].sessionID), "S%06d", i);
        snprintf(sessions[i].movieName, sizeof(sessions[i].movieName), "Movie%d", i % 3); // 假设有3个影片
        snprintf(sessions[i].cinemaName, sizeof(sessions[i].cinemaName), "Cinema%d", i % 5); // 假设有5个影院
        sessions[i].hallID = i % 5 + 1; // 假设每个影院有5个影厅
        snprintf(sessions[i].startTime, sizeof(sessions[i].startTime), "%02d:%02d", i % 24, 0);
        snprintf(sessions[i].endTime, sizeof(sessions[i].endTime), "%02d:%02d", (i % 24) + 2, 0); // 每场电影2小时
        sessions[i].duration = 120; // 电影时长2小时
        sessions[i].remainingSeats = 100 - i; // 假设余票数递减
        sessions[i].ticketPrice = 50 + i * 5; // 票价递增
        snprintf(sessions[i].language, sizeof(sessions[i].language), "Lang%d", i);
    }
}
//用户登录函数
int userLogin() {
    printf("用户登录...\n");
    char inputUserID[11];
    char inputPassword[51];
    int chance = 3;
    int userFound = 0;
    for (int j = 1; j <= 3; j++) {
        printf("请输入用户ID: ");
        scanf("%s", inputUserID);
        printf("请输入密码: ");
        scanf("%s", inputPassword);

        int i_chw=0;
        for (int i_chw = 0; i_chw < 100; i_chw++) {
            if (strcmp(users[i_chw].userID, inputUserID) == 0 && strcmp(users[i_chw].password, inputPassword) == 0) {
                printf("用户登录成功！\n");
                userFound = 1;
                break;
            }
        }

        if (userFound) {
            return i_chw;
        } else {
            printf("用户登录失败,用户ID或密码错误,请重试,还剩%d次机会。\n", chance - j);
            if ((chance - j) == 0) {
                 printf("用户登录失败, 用户ID或密码错误\n");
                exit(0); // 登录失败，返回0
            }
        }
    }
    return 0;
}
//管理员登陆函数
int adminLogin() {
    printf("管理员登录...\n");
    char inputAdminID[11];
    char inputPassword[51];
    int chance = 3;
    int adminFound = 0;

    for (int j = 1; j <= 3; j++) {
        printf("请输入管理员ID: ");
        scanf("%s", inputAdminID);
        printf("请输入密码: ");
        scanf("%s", inputPassword);
        int i_adm;
        for ( i_adm=0; i_adm < 5; i_adm++) {
            if (strcmp(admins[i_adm].adminID, inputAdminID) == 0 && strcmp(admins[i_adm].password, inputPassword) == 0) {
                printf("管理员登录成功！\n");
                adminFound = 1;
                break;
            }
        }

        if (adminFound) {
            return i_adm;
            break;
        } else {
            printf("管理员登录失败,管理员ID或密码错误,请重试,还剩%d次机会。\n", chance - j);
            if ((chance - j) == 0) {
                printf("管理员登录失败,管理员ID或密码错误\n");
                exit(0);
            }
        }
    }
    return 0;
}

// 查询场次函数
void queryTodaySessions(struct MovieSession sessions[], int count, const char* input) {
    int found = 0;
    printf("今天关于'%s'的场次有:\n", input);

    for (int i = 0; i < count; ++i) {
        char combined[102];
        snprintf(combined, sizeof(combined), "%s %s", sessions[i].movieName, sessions[i].cinemaName);

        if (strcmp(sessions[i].movieName, input) == 0 ||
            strcmp(sessions[i].cinemaName, input) == 0 ||
            strcmp(combined, input) == 0) {

            printf("场次ID: %s, 影院: %s, 时间: %s-%s, 剩余座位: %d, 票价: %.2f, 语言: %s\n",
                   sessions[i].sessionID, sessions[i].cinemaName, sessions[i].startTime, sessions[i].endTime, 
                   sessions[i].remainingSeats, sessions[i].ticketPrice, sessions[i].language);

            found = 1;
        }
    }

    if (!found) {
        printf("当前提供信息未能查询到结果。\n");
    }
}






//找到相应的电影院的所有场次函数
void querySessionsByAdminID(const char* adminID, struct Administrator admins[], int adminCount, struct MovieSession sessions[], int *sessionCount) {

    char cinemaName[51];
    for (int i = 0; i < adminCount; i++) {
        if (strcmp(adminID, admins[i].adminID) == 0) {
            strcpy(cinemaName, admins[i].cinemaName);
            break;
        }
    }
    printf("影院场次: %s\n", cinemaName);
    for (int i = 0; i < *sessionCount; i++) {
        if (strcmp(cinemaName, sessions[i].cinemaName) == 0) {
            printf("场次ID: %s\n", sessions[i].sessionID);
            printf("影厅ID: %d\n", sessions[i].hallID);
            printf("开始时间: %s\n", sessions[i].startTime);
            printf("结束时间: %s\n", sessions[i].endTime);
            printf("持续时间: %d minutes\n", sessions[i].duration);
            printf("剩余时间: %d\n", sessions[i].remainingSeats);
            printf("票价: %.2f\n", sessions[i].ticketPrice);
            printf("语言: %s\n\n", sessions[i].language);
        }
    }
    return;
}
//个人信息显示
void displayUserInfo(int U ){
    int n=0;
    int a=U;
    printf("显示用户信息...\n");
    printf("User ID: %s\n", users[a].userID);
    printf("Name: %s\n", users[a].name);
    printf("Gender: %c\n", users[a].gender);
    printf("Phone: %s\n", users[a].phone);
    printf("Email: %s\n", users[a].email);
    printf("Balance: %.2f\n", users[a].balance);
    printf("1.编辑信息\t2.订单信息\t3.返回");
    scanf("%d",&n);
    if(n == 1){
        modifyUserInfo(Uresult);
    }else if(n == 2){
        displayOrder();
    }
}
//个人信息修改
void modifyUserInfo(int Uresult) {
    int b=0;
    int a=Uresult;
    printf("修改用户信息...\n");
    printf("请选择需要改变的信息\n");
    printf("1.Name\n2.Gender\n3.Phone\n4.Email\n5.password");
    scanf("%d",&b);
    void clearInputBuffer();
    switch (b) {
                    case 1:
            printf("当前Name: %s\n", users[Uresult].name);
            printf("请输入新的Name (最多50个字符)：");
            scanf("%50s", &users[Uresult].name);
            printf("修改后Name: %s\n", users[Uresult].name);
            break;
        case 2:
            printf("当前Gender: %c\n", users[Uresult].gender);
            printf("请输入新的Gender (M 或 F)：");
            scanf(" %c", &users[Uresult].gender); 
            printf("修改后Gender: %c\n", users[Uresult].gender);
            break;
        case 3:
            printf("当前Phone: %s\n", users[Uresult].phone);
            printf("请输入新的Phone (11位数字)：");
            scanf("%11s", &users[Uresult].phone);
            printf("修改后Phone: %s\n", users[Uresult].phone);
            break;
        case 4:
            printf("当前Email: %s\n", users[Uresult].email);
            printf("请输入新的Email (最多50个字符)：");
            scanf("%50s", &users[Uresult].email);
            printf("修改后Email: %s\n", users[Uresult].email);
            break;
        case 5:
            printf("当前Password: %s\n", users[Uresult].password);
            printf("请输入新的Password (最多50个字符)：");
            scanf("%50s", &users[Uresult].password);
            printf("修改后Password: %s\n", users[Uresult].password);
            break;
        default:
            printf("无效选择，请重试。\n");

                    displayUserInfo(Uresult);
    }
}

void searchMovie() {
    printf("搜索查询影片...\n");
    printf("请输入想要查询的影片名或者影院名（或者输入“影片名 影院名”的格式）:\n");
    // printf("按回车键返回上一步\n");
    // getchar();
    // if (getchar() != '\n') {
    //      return;
    //  }
}

void displayOrder() {
    printf("显示订单...\n");
}

void manageOrder() {
    printf("订单管理...\n");
}


void showMovies() {
    printf("显示影片...\n");
}



//影厅管理wc
// 管理影厅的函数
void manageHall(struct Hall halls[], int *hallCount) {
    int choice;
    do {
        printf("\n影厅管理功能:\n");
        printf("1. 查询影厅\n");
        printf("2. 添加影厅\n");
        printf("3. 返回\n");
        printf("请选择操作: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                showHalls(halls, *hallCount); // 显示所有影厅信息
                break;
            case 2:
                addHall(halls, hallCount); // 添加新的影厅
                break;
            case 3:
                printf("返回管理员功能菜单。\n");
                break;
            default:
                printf("输入无效，请重试。\n");
                break;
        }
    } while (choice != 3);
}
// 显示指定影厅的详细信息
void showHallInfo(const struct Hall *hall) {
    printf("影厅ID: %d\n", hall->hallID);
    printf("总座位数: %d\n", hall->totalSeats);
    printf("影厅类型: %s\n", hall->hallType);
}

//显示所有影厅信息
void showHalls(const struct Hall halls[], int size) {
     printf("当前影厅信息:\n");
    for (int i = 0; i < size; i++) {
        printf("影厅ID: %d\n", halls[i].hallID);
        printf("总座位数: %d\n", halls[i].totalSeats);
        printf("影厅类型: %s\n\n", halls[i].hallType);
    }
}
// 添加新的影厅
void addHall(struct Hall halls[], int *size) {
    if (*size >= 100) {
        printf("影厅数量达到上限，无法添加新影厅。\n");
        return;
    }

    struct Hall newHall;
    printf("请输入影厅ID: ");
    scanf("%d", &newHall.hallID);
    printf("请输入总座位数: ");
    scanf("%d", &newHall.totalSeats);
    printf("请输入影厅类型: ");
    scanf("%s", newHall.hallType);

    halls[(*size)++] = newHall; // 添加新的影厅并更新数量
    printf("影厅已添加。\n");
    
    // 保存影厅数据到文件 魏澄
    saveHallsToFile();
}

//场次管理chw
//场次管理函数声明
void manageSessions(struct MovieSession sessions[], int *sessionCount) {
    printf("场次管理...\n");
    printf("1. 查询场次\n2. 添加场次\n3. 返回主菜单\n");
    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            querySessionsByAdminID(admins[Aresult].adminID, admins, 5, sessions, sessionCount); //如后续需要添加管理员，这里需要指针
            break;
        case 2:
            addSession(sessions, sessionCount);
            break;
        case 3:
            return;
        default:
            printf("无效选择，请重试。\n");
    }
}
//添加场次的函数
void addSession(struct MovieSession sessions[], int *sessionSize) {
    if (*sessionSize >= 100) {
        printf("场次数量达到上限，无法添加新影厅。\n");
        return;
    }
    
    struct MovieSession newSession; //用来添加场次的结构体
    printf("添加场次...\n");
    printf("请输入场次ID (6-10字符): ");
    scanf("%10s", newSession.sessionID);
    printf("请输入影片名 (最多50个字符): ");
    scanf("%50s", newSession.movieName);
    printf("请输入影院名 (最多50个字符): ");
    scanf("%50s", newSession.cinemaName);
    printf("请输入影厅ID: ");
    scanf("%d", &newSession.hallID);
    printf("请输入开始时间 (格式hh:mm): ");
    scanf("%5s", newSession.startTime);
    printf("请输入结束时间 (格式hh:mm): ");
    scanf("%5s", newSession.endTime);
    printf("请输入电影时长 (分钟): ");
    scanf("%d", &newSession.duration);
    printf("请输入场次余票数: ");
    scanf("%d", &newSession.remainingSeats);
    printf("请输入票价: ");
    scanf("%f", &newSession.ticketPrice);
    printf("请输入放映类型 (最多10个字符): ");
    scanf("%10s", newSession.language);
    // 将新场次添加到sessions数组中
    
    sessions[(*sessionSize)++] = newSession; // 添加新的影厅并更新数量
    printf("影厅已添加。\n");
}

void showSessions() {
    printf("显示场次...\n");
}

void managePersonalInfo() {
    printf("个人信息管理...\n");
}


//清空缓冲区
void clearInputBuffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}