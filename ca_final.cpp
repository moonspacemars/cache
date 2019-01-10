#include<iostream>
#include<fstream>
#include<vector>
#include<math.h>


using namespace std;



class UnitCell{
    private:
        bool valid;
        int tag,index;
    public:
        UnitCell():valid(false),tag(0),index(0){};
        UnitCell(int idx):index(idx){
            valid=false;
            tag=-1;
        }
        int  GetTag(){ return tag; }
        int  GetIndex(){return index;}
        bool GetValid(){return valid;}
        void SetValid(bool v){valid=v;}
        void SetTag(int t){tag=t;}

};

int BStrToInt(string str){
    int sum=0;
    for (int i=0; i<str.size(); i++){
        if (i==str.size()-1){
            if  (str[i]=='1')
                sum=sum+1;
        }
        else{       

            if (str[i]=='1')
                sum=2*(sum+1);
            else
                sum=2*sum;
        }
    }

    return sum;
    
}

int main(int argc, char *argv[]){

    string filePath, outputPath;
    vector<string> refGroup;

    int addressBits=0, numSet=0, associate=0, blockSize=0,idxBitCount=0, offsetBitCount=0;
    
   //read file
    for (int i=1; i<argc; i++){
  

        
        filePath="";
        filePath.assign(argv[i]);

    
        ifstream inputfile(filePath);

        if (i==1|| i==2)    
            if (!inputfile)
                cout<<"Can't open file!\n";
    
        if (i==1){ //cache.org
            char buf;
            int row=0;
            string tmpstr;


            while(inputfile>>buf){
                int a=0;
                if (buf==58){//:
                    row++;
                    inputfile>>a;
                    if (row==1)
                        addressBits=a;
                    else if (row ==2 )
                        numSet=a;
                    else if (row==3)
                        associate=a;
                    else
                        blockSize=a;                        
                }
            }  
        }
        else if (i==2){ //ref list
            string tmpstr;
            int row=0;
            while(getline(inputfile,tmpstr)){
                /*row++;
                if ((row==1) ||(tmpstr==".end"))
                    continue;*/

                refGroup.push_back(tmpstr);
            }

        }
        else if (i==3){ //output file path
            outputPath=filePath;
        }
       
    }







/*cout<<addressBits<<endl;
cout<<numSet<<endl;
cout<<associate<<endl;
cout<<blockSize<<endl;*/

/*
for (int x=0; x<refGroup.size(); x++){
    cout<<refGroup[x]<<endl;
}*/

//cout<<outputPath<<endl;

idxBitCount=log2(numSet);

offsetBitCount=log2(blockSize);

//cout<<"idx"<<idxBitCount<<endl;

//cout<<"off"<<offsetBitCount<<endl;
vector<vector<UnitCell>> setGroup;
setGroup.resize(numSet);
/*
for (int set_idx=0; set_idx<numSet; set_idx++){
    for (int way_idx=0; way_idx<associate; way_idx++){
        UnitCell iCell(set_idx);
        setGroup[set_idx].push_back(iCell);
    }
    
}*/


int tagCount=addressBits-offsetBitCount-idxBitCount;
int idxInt=0,tagInt=0;

vector<bool> hitGroup;

hitGroup.resize(refGroup.size());

string fulladdr,tagbit_str,idxbit_str;
for (int ref_idx=1; ref_idx<refGroup.size()-1; ref_idx++){
    fulladdr=refGroup[ref_idx];
    idxbit_str=idxbit_str.assign(fulladdr, tagCount, idxBitCount );
    //cout<<"idxbit: "<<idxbit_str<<endl;
    tagbit_str=tagbit_str.assign(fulladdr, 0, tagCount );
    //cout<<"tagbit: "<<tagbit_str<<endl;
    idxInt=BStrToInt(idxbit_str);
    tagInt=BStrToInt(tagbit_str);
    bool finishCheck=false;
    bool isHit=false;
    for (int way_idx=0; way_idx<setGroup[idxInt].size(); way_idx++){
        //cout<<"idx: "<<idxInt<<endl;
        //UnitCell tmpcell=setGroup[idxInt][way_idx];
        /*if (idxInt==4){
            cout<<"ref: "<<ref_idx<<"tag: "<<tagInt<<endl;
        }*/

        if (setGroup[idxInt][way_idx].GetValid()){ // is valid

        //cout<<"VALID"<<endl;
            if (setGroup[idxInt][way_idx].GetTag()==tagInt){
                finishCheck=true;
                isHit=true;
                setGroup[idxInt].erase(setGroup[idxInt].begin()+way_idx);
                UnitCell newCell(idxInt);
                newCell.SetValid(true);
                newCell.SetTag(tagInt);
                setGroup[idxInt].push_back(newCell);

                break;
            }
         
        }
       /* else{
            //cout<<"N V"<<endl;
            setGroup[idxInt][way_idx].SetValid(true);
            setGroup[idxInt][way_idx].SetTag(tagInt);
            finishCheck=true;
            break;
            
        }*/
    }

    if (!finishCheck){
        //setGroup[idxInt].erase(setGroup[idxInt].begin());
        UnitCell newCell(idxInt);
        newCell.SetValid(true);
        newCell.SetTag(tagInt);
        setGroup[idxInt].push_back(newCell);
    }

    if (setGroup[idxInt].size()>associate)
        setGroup[idxInt].erase(setGroup[idxInt].begin());        

    hitGroup[ref_idx]=isHit;

}


/*
for (int x=0; x<hitGroup.size(); x++){
    cout<<hitGroup[x]<<endl;
}*/

    ofstream answerOutput (outputPath);
    if (answerOutput.is_open()){    
   
            answerOutput<<"Address bits: "<<addressBits<<endl;
            answerOutput<<"Number of sets: "<<numSet<<endl;
            answerOutput<<"Associativity: "<<associate<<endl;
            answerOutput<<"Block size: "<<blockSize<<endl;


        answerOutput<<endl;

        answerOutput<<"Indexing bit count: "<<idxBitCount<<endl;
        answerOutput<<"Indexing bits: ";

        for (int i=0; i<idxBitCount;  i++){
            answerOutput<<addressBits-tagCount-i-1<<" ";    
        }
        answerOutput<<endl;
        answerOutput<<"Offset bit count: "<<offsetBitCount<<endl;
        answerOutput<<endl;    

        string tmpstr;
        int missCount=0;
        for (int i=0; i<refGroup.size(); i++){
            
            
            
            if (i>0 && i<refGroup.size()-1){
                tmpstr=tmpstr.assign(refGroup[i],0, refGroup[i].size()-1);            
                answerOutput<<tmpstr;
                if (hitGroup[i])
                    answerOutput<<" hit";
                else{
                    answerOutput<<" miss";
                    missCount++;
                }
            }
            else
                answerOutput<<refGroup[i];
            answerOutput<<endl;
        }

        answerOutput<<endl;


        answerOutput<<"Total cache miss count: "<<missCount<<endl;




    }
    else cout<<"unable to open file!";



    return 0;
}