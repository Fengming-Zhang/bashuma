#include <iostream>
#include <cmath>
using namespace std;
#define max_open_num  10000
#define max_close_num 10000
#define max_path_num 10000
/////////////////////////////////
//          �ṹ�嶨��
/////////////////////////////////
typedef struct {
    int num[3][3];
} puzzle;

typedef struct {
    puzzle state;
    int depth;   //�ڵ�����
    int dist;    //��Ŀ��������پ���
    int fx;      //��������fx=depth+dist
} Open;

typedef struct {
    Open note[max_open_num];
}  Openlist;

typedef struct {
    puzzle state;
    int fx;
} Close;

typedef struct {
    Close note[max_close_num];
} Closelist;

typedef struct {
    Open pa[max_path_num];
} Path;
/////////////////////////////////
//          ��������
/////////////////////////////////
int once_sort(Openlist &op,int start,int end);
bool puzzle_equal(puzzle a,puzzle b);
void deletefirst(Openlist &openlist,int num);
int get_dist(puzzle a,puzzle b);
void print_puzzle(puzzle a);
void printpath(Path path,int num);
int locate_zero(puzzle a);
void copy_puzzle(puzzle &a,puzzle b);
void create_sub(Open sub[],Open cur);
void calc_open(Open &child,Open father,puzzle end);
bool exist_in_open(puzzle a,Openlist op,int num);
bool exist_in_close(puzzle a,Closelist cl,int num);
void listsort(Openlist &op,int num);
int once_sort(Openlist &op,int start,int end);
void search(puzzle start,puzzle end);
/////////////////////////////////
//        �ж�puzzle�Ƿ����
/////////////////////////////////
bool puzzle_equal(puzzle a,puzzle b){
    for(int i=0;i<3;i++)
       for(int j=0;j<3;j++)
           if(a.num[i][j]!=b.num[i][j])
              return false;
    return true;
}
/////////////////////////////////
//       ɾ��openlist��ͷ
/////////////////////////////////
void deletefirst(Openlist &openlist,int num){
    for(int i=0;i<num;i++)
      openlist.note[i]=openlist.note[i+1];
}
/////////////////////////////////
//       �����پ���
/////////////////////////////////
int get_dist(puzzle a,puzzle b){
    int cnt=0;
    for(int i=0;i<9;i++){
        //if(b.num[i/3][i%3]!=0)
        for(int j=0;j<9;j++)
            if(a.num[j/3][j%3]==b.num[i/3][i%3]) cnt+=(abs(i%3-j%3)+abs(i/3-j/3));
    }
    return cnt;
}/*
int get_dist(puzzle a,puzzle b){
    int cnt=0;
    for(int i=0;i<9;i++)
        if(a.num[i/3][i%3]!=b.num[i/3][i%3]) cnt++;
    return cnt;
}*/
/////////////////////////////////
//       ��ӡpuzzle
/////////////////////////////////
void print_puzzle(puzzle a){
    for(int i=0;i<3;i++){
       for(int j=0;j<3;j++)
           if(a.num[i][j]>0) cout<<a.num[i][j]<<" ";
           else cout<<"  ";
       cout<<endl;
    }
}
/////////////////////////////////
//       �ж��Ƿ��н�
/////////////////////////////////
bool have_solution(puzzle a,puzzle b){
    int cnt_a=0,cnt_b=0;
    for(int i=1;i<9;i++)
    for(int j=0;j<i;j++){
        if(a.num[j/3][j%3]>a.num[i/3][i%3]&&a.num[j/3][j%3]!=0&&a.num[i/3][i%3]!=0) cnt_a++;
        if(b.num[j/3][j%3]>b.num[i/3][i%3]&&b.num[j/3][j%3]!=0&&b.num[i/3][i%3]!=0) cnt_b++;
    }
    if(cnt_a%2==cnt_b%2) return 1;
    return 0;
}
/////////////////////////////////
//        ��ӡ·��
/////////////////////////////////
void printpath(Path path,int num){
    int cur=num-1,cnt=1;
    Open *procedure;
    procedure=new Open[num];
    procedure[0]=path.pa[cur];
    while(cur>0){
        for(int i=cur;i>=0;i--){
            if(get_dist(path.pa[i].state,path.pa[cur].state)==2){
              cur=i;
              procedure[cnt++]=path.pa[i];
              break;
            }
        }
    }
    for(int i=cnt-1;i>=0;i--){
        cout<<"step:"<<cnt-i-1<<endl;
        print_puzzle(procedure[i].state);
    }
    cout<<"��������:\n";
    cout<<"�ܲ���Ϊ��"<<cnt-1<<endl;
}
/////////////////////////////////
//        ��0��λ��
/////////////////////////////////
int locate_zero(puzzle a){
    for(int i=0;i<9;i++)
        if(a.num[i/3][i%3]==0) return i;
    return 0;
}
/////////////////////////////////
//        ����puzzle
/////////////////////////////////
void copy_puzzle(puzzle &a,puzzle b){
    for(int i=0;i<3;i++)
       for(int j=0;j<3;j++)
         a.num[i][j]=b.num[i][j];
}
/////////////////////////////////
//        �ҵ��ڵ�����з�֧
/////////////////////////////////
void create_sub(Open sub[],Open cur){
    int num=0,location;
    location=locate_zero(cur.state);
    //up
    if(location>2){
        copy_puzzle(sub[num].state,cur.state);
        sub[num].state.num[location/3][location%3]=cur.state.num[location/3-1][location%3];
        sub[num].state.num[location/3-1][location%3]=cur.state.num[location/3][location%3];
        sub[num].depth=1;
        num++;
    }
    //right
    if(location%3!=2){
        copy_puzzle(sub[num].state,cur.state);
        sub[num].state.num[location/3][location%3]=cur.state.num[location/3][location%3+1];
        sub[num].state.num[location/3][location%3+1]=cur.state.num[location/3][location%3];
        sub[num].depth=1;
        num++;
    }
    //down
    if(location<6){
        copy_puzzle(sub[num].state,cur.state);
        sub[num].state.num[location/3][location%3]=cur.state.num[location/3+1][location%3];
        sub[num].state.num[location/3+1][location%3]=cur.state.num[location/3][location%3];
        sub[num].depth=1;
        num++;
    }
    //left
    if(location%3!=0){
        copy_puzzle(sub[num].state,cur.state);
        sub[num].state.num[location/3][location%3]=cur.state.num[location/3][location%3-1];
        sub[num].state.num[location/3][location%3-1]=cur.state.num[location/3][location%3];
        sub[num].depth=1;
        num++;
    }
}
/////////////////////////////////
//      ����Open�ڵ��еĲ���
/////////////////////////////////
void calc_open(Open &child,Open father,puzzle end){
    child.depth=father.depth+1;
    child.dist=get_dist(child.state,end);
    child.fx=child.dist+child.depth;
}
/////////////////////////////////
//      ����openlist�Ƿ��е�ǰ�ڵ�
/////////////////////////////////
bool exist_in_open(Open a,Openlist &op,int num){
    for(int i=0;i<num;i++)
      if(puzzle_equal(a.state,op.note[i].state))  {
            op.note[i].fx=min(op.note[i].fx,a.fx);
            return true;
      }
    return 0;
}
/////////////////////////////////
//      ����closelist�Ƿ��е�ǰ�ڵ�
/////////////////////////////////
bool exist_in_close(puzzle a,Closelist cl,int num){
    for(int i=0;i<num;i++)
        if(puzzle_equal(a,cl.note[i].state)) return true;
    return 0;
}/*
void listsort(Openlist &op,int start,int end){
    int mid;
    if(start<end){
      mid=once_sort(op,start,end);
      listsort(op,start,mid-1);
      listsort(op,mid+1,end);
    }
}
int once_sort(Openlist &op,int start,int end){
    int left=start,right=end;
    Open temp=op.note[start];
    while(left<right){
        while(op.note[right].fx>temp.fx)  right--;
        if(right>left) op.note[left]=op.note[right];
        while(op.note[left].fx<temp.fx)  left++;
        if(right>left) op.note[right]=op.note[left];
    }
    op.note[left]=temp;
    return left;
}
*/
/////////////////////////////////
//  openlist��������������С��������
/////////////////////////////////
void listsort(Openlist &op,int num){
    Open temp;
    int index;
    for(int i=0;i<num;i++){
        index=i;
        for(int j=i+1;j<num;j++){
            if(op.note[j].fx<op.note[index].fx){
                temp=op.note[index];
                op.note[index]=op.note[j];
                op.note[j]=temp;
            }
        }
    }
}
/////////////////////////////////
//          ����·��
/////////////////////////////////
void search(puzzle start,puzzle end){
    int open_num=0,close_num=0,path_num=0;
    Openlist openlist;
    Closelist closelist;
    Open nop;
    Path path;
    openlist.note[open_num].state=start;
    openlist.note[open_num].depth=0;
    openlist.note[open_num].dist=get_dist(start,end);
    openlist.note[open_num].fx=openlist.note[open_num].dist+openlist.note[open_num].depth;            //��ʼ��openlist
    open_num++;
    while(open_num!=0){                       //open��Ϊ��
        nop=openlist.note[0];                 //nopΪopenlistͷ���
        path.pa[path_num++]=nop;              //����ǰ�ڵ����·��
        deletefirst(openlist,open_num);
        open_num--;
        if(puzzle_equal(nop.state,end)){
            printpath(path,path_num);
            break;
        }
        Open sub[4];
        create_sub(sub,nop);
        for(int i=0;i<4;i++){
            if(sub[i].depth==1){
                calc_open(sub[i],nop,end);
                if(!exist_in_open(sub[i],openlist,open_num)&&!exist_in_close(sub[i].state,closelist,close_num)){                  //�ڵ㲻��openlist��closelist�У����ڵ����openlist
                    openlist.note[open_num]=sub[i];
                    open_num++;
                    continue;
                }
                if(exist_in_open(sub[i],openlist,open_num))   continue;                 //�ڵ���openlist��������openlist�д˽ڵ����������fx
                if(exist_in_close(sub[i].state,closelist,close_num))  continue;          //�ڵ���closelist������
            }
        }
        closelist.note[close_num].state=nop.state;
        closelist.note[close_num].fx=nop.fx;
        close_num++;                                                                      //��nop����closelist
        listsort(openlist,open_num);
    }
}
int main()
{
    puzzle start,end;
    cout<<"����������\n";
    cout<<"����������\n";
    cout<<"���� 1 2 3 4 0 5 6 7 8 \n";
    cout<<"���ʾ����Ҫ��ʾ�İ�����Ϊ:\n";
    cout<<"\t1 2 3\n\t4   5\n\t6 7 8\n";
    cout<<"ע�⣬����֮��Ҫ�пո������������Ϊ����0����\n";
    cout<<"�������ʼ״̬:)\n";
    for (int i=0;i<3;i++)
       for(int j=0;j<3;j++)
         cin>>start.num[i][j];
    cout<<"����ĳ�ʼ״̬Ϊ:\n";
    print_puzzle(start);
    cout<<"������Ŀ��״̬:\n";
    for (int i=0;i<3;i++)
       for(int j=0;j<3;j++)
         cin>>end.num[i][j];
    cout<<"�����Ŀ��״̬Ϊ:\n";
    print_puzzle(end);
    if(have_solution(start,end)){
        cout<<"������ʼ:\n";
        search(start,end);
        cout<<"Success"<<endl;
    }
    else cout<<"No solution!"<<endl;
    return 0;
}
