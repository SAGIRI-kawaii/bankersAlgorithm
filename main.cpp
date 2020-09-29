#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int resourcesCount=0;       // 资源总数
int processCount=0;         // 进程总数
char requestProcessChar;    // 申请资源的进程名
int requestProcessNum;      // 申请资源的进程编号
vector<vector<int> > resourcesRequiredMax;  // 最大需求矩阵
vector<vector<int> > resourcesAllocated;    // 已分配矩阵
vector<vector<int> > resourcesNeed;         // 资源需求矩阵
vector<int> resourcesAvaliable;             // 可用资源矩阵
vector<int> resourcesRequests;              // 进程申请的资源数量
vector<int> safeSequence;                   // 安全序列

void showData(){
    int resourcesCount=resourcesAvaliable.size();
    int processCount=resourcesRequiredMax.size();
    printf("------------------信息一览------------------\n");
    printf("Avaliable:\n");
    for(int i=0;i<resourcesCount;i++){
        printf("%2c ",'A'+i);
    }
    printf("\n");
    for(int i=0;i<resourcesCount;i++){
        printf("%2d ",resourcesAvaliable[i]);
    }
    printf("\n\n\n");
    printf("Allocated:\n");
    printf("    ");
    for(int i=0;i<resourcesCount;i++){
        printf("%2c ",'a'+i);
    }
    printf("\n");
    for(int i=0;i<processCount;i++){
        printf("%2c  ",'A'+i);
        for(int j=0;j<resourcesCount;j++){
            printf("%2d ",resourcesAllocated[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
    printf("Max:\n");
    printf("    ");
    for(int i=0;i<resourcesCount;i++){
        printf("%2c ",'a'+i);
    }
    printf("\n");
    for(int i=0;i<processCount;i++){
        printf("%2c  ",'A'+i);
        for(int j=0;j<resourcesCount;j++){
            printf("%2d ",resourcesRequiredMax[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
    printf("Need:\n");
    printf("    ");
    for(int i=0;i<resourcesCount;i++){
        printf("%2c ",'a'+i);
    }
    printf("\n");
    for(int i=0;i<processCount;i++){
        printf("%2c  ",'A'+i);
        for(int j=0;j<resourcesCount;j++){
            printf("%2d ",resourcesNeed[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

bool bankersAlgorithm(int requestProcessNum,vector<int> resourcesRequests){
    int resourcesCount=resourcesRequests.size();
    int processCount=resourcesRequiredMax.size();
    int finishedCount=0;
    int processNow=-1;
    bool flag=true;

    vector<int> processFinished(processCount,false);
    vector<int> tempResourcesAvaliable(resourcesAvaliable.begin(),resourcesAvaliable.end());

    for(int i=0;i<resourcesCount;i++){
        if(resourcesRequests[i]>tempResourcesAvaliable[i]){
            return false;
        }
    }

    bool finishJudge=true;
    for(int i=0;i<resourcesCount;i++){
        if(resourcesRequests[i]!=resourcesNeed[requestProcessNum][i]){
            finishJudge=false;
            break;
        }
    }

    if(finishJudge){
        safeSequence.push_back(requestProcessNum+1);
        finishedCount=1;
        processFinished[requestProcessNum]=true;
        for(int i=0;i<resourcesCount;i++) tempResourcesAvaliable[i]+=resourcesAllocated[requestProcessNum][i];
    }else{
        for(int i=0;i<resourcesCount;i++) tempResourcesAvaliable[i]-=resourcesRequests[i];
        for(int i=0;i<resourcesCount;i++) resourcesNeed[requestProcessNum][i]-=resourcesRequests[i];
        for(int i=0;i<resourcesCount;i++) resourcesAllocated[requestProcessNum][i]+=resourcesRequests[i];
    }

    while(flag){
        flag=false;
        // printf("processNow:%d\n",processNow);
        if(processNow!=-1){
            processFinished[processNow]=true;
            processNow=-1;
            finishedCount+=1;
        }

        if(finishedCount==processCount) return true;

        bool thisCanFinish=true;

        for(int i=0;i<processCount;i++){
            if(processFinished[i]==false){
                // printf("\nprocessNow:%d",i+1);
                processNow=i;
                // printf("\nNeed:");
                for(int j=0;j<resourcesCount;j++){
                    // printf("(%c:%d %d) ",'a'+j,resourcesNeed[i][j],tempResourcesAvaliable[j]);
                    if(resourcesNeed[i][j]>tempResourcesAvaliable[j]){
                        processNow=-1;
                        thisCanFinish=false;
                        break;
                    }
                }
                if(thisCanFinish){
                    for(int j=0;j<resourcesCount;j++) tempResourcesAvaliable[j]+=resourcesAllocated[processNow][j];
                    safeSequence.push_back(processNow+1);
                    flag=true;
                    // printf("True!!!!!\n");
                    break;
                }else{
                    thisCanFinish=true;
                }
            }
        }
    }
    if(finishedCount==processCount) return true;
    return false;
}

void dataInput(){
    char _;
    printf("请输入资源数量:");
    scanf("%d",&resourcesCount);
    printf("资源已自动分配名字a-%c\n",'a'+resourcesCount-1);
    printf("请输入进程数量:");
    scanf("%d",&processCount);
    printf("进程已自动分配名字A-%c\n",'A'+processCount-1);

    int tempProcessResourcesRequiredMaxCount;
    printf("\n-----请输入各进程所需最大资源数量-----\n");
    for(int i=0;i<processCount;i++){
        vector<int>tempProcessResourcesRequiredMax;
        for(int j=0;j<resourcesCount;j++){
            printf("请输入进程%c所需资源%c最大数量:",'A'+i,'a'+j);
            scanf("%d",&tempProcessResourcesRequiredMaxCount);
            tempProcessResourcesRequiredMax.push_back(tempProcessResourcesRequiredMaxCount);
        }
        printf("-----------------------------------\n");
        resourcesRequiredMax.push_back(tempProcessResourcesRequiredMax);
    }

    int tempProcessResourcesAllocatedCount;
    printf("\n-----请输入各进程现已分配资源数量-----\n");
    for(int i=0;i<processCount;i++){
        vector<int>tempProcessResourcesAllocated;
        vector<int>tempProcessResourcesNeed;
        for(int j=0;j<resourcesCount;j++){
            printf("请输入进程%c已占有资源%c的数量:",'A'+i,'a'+j);
            scanf("%d",&tempProcessResourcesAllocatedCount);
            tempProcessResourcesAllocated.push_back(tempProcessResourcesAllocatedCount);
            tempProcessResourcesNeed.push_back(resourcesRequiredMax[i][j]-tempProcessResourcesAllocatedCount);
        }
        printf("-----------------------------------\n");
        resourcesNeed.push_back(tempProcessResourcesNeed);
        resourcesAllocated.push_back(tempProcessResourcesAllocated);
    }

    int tempResourcesAviliable;
    printf("\n-----请输入各资源空闲数量-----\n");
    for(int i=0;i<resourcesCount;i++){
        printf("资源%c空闲数量:",'a'+i);
        scanf("%d",&tempResourcesAviliable);
        resourcesAvaliable.push_back(tempResourcesAviliable);
    }


    scanf("%c",&_); //清除缓冲区内字符

    while(1){
        printf("\n请输入申请资源的进程名(英文字母):");
        scanf("%c",&requestProcessChar);
        requestProcessNum=requestProcessChar-'A';
        if(requestProcessNum>=processCount || requestProcessNum<0){
            printf("输入错误！无进程%c！请重新输入！\n",requestProcessChar);
            scanf("%c",&_); //清除缓冲区内字符
        }
        else break;
    }

    printf("\n-----请输入进程%c申请的资源数量-----\n",requestProcessChar);
    int tempRequestResourceCount;
    for(int i=0;i<resourcesCount;i++){
        while(1){
            printf("请输入进程%c申请资源%c的数量:",requestProcessChar,'a'+i);
            scanf("%d",&tempRequestResourceCount);
            if(tempRequestResourceCount+resourcesAllocated[requestProcessNum][i]>resourcesRequiredMax[requestProcessNum][i]){
                printf("错误！进程%c申请资源%c数量加上进程%c已被分配资源%c的数量超过进程%c所需最大资源%c数量！请检查后重新输入！\n",requestProcessChar,'a'+i,requestProcessChar,'a'+i,requestProcessChar,'a'+i);
            }else{
                resourcesRequests.push_back(tempRequestResourceCount);
                break;
            }
        }
    }
}

int main(){
    dataInput();
    showData();
    if(bankersAlgorithm(requestProcessNum,resourcesRequests)){
        printf("Safe!Security sequence:");
        for(int i=0;i<safeSequence.size();i++)  printf(" %d",safeSequence[i]);
    }else{
        printf("Unsafe!Can not allocate!\n");
        for(int i=0;i<safeSequence.size();i++)  printf(" %d",safeSequence[i]);
    }
    return 0;
}
