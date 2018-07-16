#include "tree.h"



int main(int argc, char const *argv[])
{

    ifstream inFS(argv[1]) ;   // Input file stream
   
    // Try to open file
    cout << "Opening file myfile.txt." << endl;
   

    if (!inFS.is_open()) {
        cout << "Could not open file argv1" << endl;
        return 1; // 1 indicates error
    }
    // attribute list creation
    string firstline;
    getline(inFS, firstline);
    vector <string> categories;
    string temp;
    for (int i = 0; i < firstline.size(); i++){
        if (firstline[i] == ','){
            categories.push_back(temp);
            temp = "";
        }
        else if(firstline[i] == ' '){}
        else{
            temp += firstline[i];
        }
    }

    // initial set creation
    int dependent_var = 0;
    vector<string> temp_parts;
    vector <int> classification;
    string buffer;
    temp = "";
    vector <sample> original_set;
    string name;

    while (getline(inFS, buffer)){
        for (int i = 0; i < buffer.size() - 2; i++){
            if (buffer[i] == ','){
                temp_parts.push_back(temp);
                temp = "";
            }
            else if (buffer[i] == ' '){}
            else{
                temp += buffer[i];
            }
        }
        char val = buffer[buffer.size() - 2];
        if (val == '1'){
            dependent_var = 1;
        }
        else{
            dependent_var = 0;
        }
        temp_parts.erase(temp_parts.begin());
        temp_parts.erase(temp_parts.begin());
        original_set.emplace_back(sample(dependent_var,temp_parts, (short) temp_parts.size()));
        temp_parts.clear();
        buffer.clear();
        temp.clear();
    }

    vector <bool> used (2, false);
    //struct node * newnode( vector <sample> s, vector <bool>  u_a, int samplesize, int boolcount)
    node * root = newnode(original_set, used, (short) original_set.size(), (short) used.size());

    root = split(root);

    root = deletetree(root);


   // decision_tree tree = decision_tree(categories, original_set, att_options);




    
    return 0;
}


//class sample{
//    private:
//        int truth_value;
//        vector<string> particulars;
//        int part_size;
//    public:
//        sample(int tf, vector<string> Att_S)
//        : truth_value(tf), particulars(Att_S) {
//            //particulars.resize((short)Att_S.size());
//            //for (int i = 0; i < (short) Att_S.size(); i--){
//              //  particulars[i] = (Att_S[i]);
//            //}
//            part_size = particulars.size();
//        }
//        string get_particular(int index){
//            return particulars[index];
//        }
//        int get_part_size(){
//            return part_size;
//        }
//        int get_truth_value(){
//            return truth_value;
//        }
//        void set_particular(string s, int index){
//            particulars[index] = s;
//        }
//};