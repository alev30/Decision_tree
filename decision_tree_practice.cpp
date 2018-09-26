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

    char val;


    inFS.clear();
    inFS.seekg(0, ios::beg);

    long linecount = std::count(std::istreambuf_iterator<char>(inFS),
               std::istreambuf_iterator<char>(), '\n');


    long lastcounter = 0;

    inFS.clear();
    inFS.seekg(0, ios::beg);

    getline(inFS, buffer);
    while (getline(inFS, buffer)){
        // issue with reading the last line fo the file, creating conditional statement to deal with this
        if (lastcounter  == linecount - 1 ){
            //cout << buffer << endl;
            for (int i = 0 ; i < buffer.size() - 2; i++){
                if (buffer[i]== ','){
                    temp_parts.push_back(temp);
                    temp = "";
                }
                else if(buffer [i] == ' '){
                    //do nothing
                }
                else{
                    temp+= buffer[i];
                }
            }
            temp_parts.push_back(temp);
            val = buffer[buffer.size() - 1];
            if (val == '1') {
                dependent_var = 1;
            } else {
                dependent_var = 0;
            }
            temp_parts.erase(temp_parts.begin()); // erases first two independent variables, only care about second two
            temp_parts.erase(temp_parts.begin());
            original_set.emplace_back(sample(dependent_var, temp_parts, (short) temp_parts.size()));
            break;
        }
        else {


            for (int i = 0; i < buffer.size() - 2; i++) {
                if (buffer[i] == ',') {
                    temp_parts.push_back(temp);
                    temp = "";
                } else if (buffer[i] == ' ') {}
                else {
                    temp += buffer[i];
                }
            }
            val = buffer[buffer.size() - 2];
            if (val == '1') {
                dependent_var = 1;
            } else {
                dependent_var = 0;
            }
            temp_parts.erase(temp_parts.begin());
            temp_parts.erase(temp_parts.begin());
            original_set.emplace_back(sample(dependent_var, temp_parts, (short) temp_parts.size()));
            temp_parts.clear();
            buffer.clear();
            temp.clear();
        }
        lastcounter++;
    }

    //cout << original_set[399].part_size << " " << original_set[399].truth_value << endl;

    vector <bool> used (2, false);
    //struct node * newnode( vector <sample> s, vector <bool>  u_a, int samplesize, int boolcount)
    node * root = new node;
    root->value = "Root";
    root->setsize = original_set.size();
    for (int i = 0; i < original_set.size(); i++){
        root->set.push_back(original_set[i]);
    }
    root->children = nullptr;
    for (int i = 0; i < (short)used.size(); i++){
        root->used_attributes.push_back(false);
    }

    //newnode(original_set, used, (short) original_set.size(), (short) used.size());

    //for ( int i = 0 ; i < (*root->set).size(); i++) {
      //  cout<< "truth value: " << (*root->set)[i].truth_value << " particular 1: " << (*root->set)[i].particulars[0] << endl;
    //}


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