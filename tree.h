//
// Created by Adam Levin on 7/13/18.
//

#ifndef DECISION_TREE_ALGORITHM_MODEL_TREE_H
#define DECISION_TREE_ALGORITHM_MODEL_TREE_H


#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>


using namespace std;


/*
split (node,{examples}) :
    1. A <- best attribute for splitting the examples (based on max entropy gain)
    2. Decision Attribute for this node <- A
    3. for each value of A, create a new child node
    4. split training {examples} to child nodes
    5. for each child node / subset
        if subset is pure, stop
        else Split (child node, {subset})
*/



class sample{
public:
    int truth_value;
    int part_size;
    vector <string>  particulars;
    sample (){}

    sample (int tf, vector<string> parts, int p_size): truth_value(tf), part_size(p_size){
        particulars = vector <string> (p_size);
        for (int i = 0; i < p_size; i++){
            particulars[i] = parts[i];
        }
    }
};



vector <vector <string> > options_create(vector <sample> & orig_set){
    int part_size = orig_set[0].part_size;
    vector <vector <string> > output(part_size);
    vector <string> temp(part_size);
    bool already_there;
    for (int i = 0; i < (short) orig_set.size(); i++){
        for (int j = 0; j < part_size; j++){
            temp[j] = orig_set[i].particulars[j];
        }
        if (i == 0){
            for (int j = 0; j < part_size; j++){
                output[j].push_back(temp[j]);
            }
        }
        else{

            for (int j = 0; j < part_size; j++){
                already_there = false;
                for (int k = 0; k < (short) output[j].size(); k ++){
                    if (output[j][k] == temp[j]){
                        already_there = true;
                    }
                }
                if (!already_there){
                    output[j].push_back(temp[j]);
                }
            }
        }
    }

//    //Standardization of values (to be completed later)//
//
//    //First, find the respective means//
//    vector <double> average(part_size,0.0);
//    string typecheck;
//    for (int i =0 ; i< part_size; i++){
//        for (int j = 0; j < (short) output[i].size(); j++){
//            typecheck.clear();
//            typecheck = output[i][j];
//            if (isdigit(typecheck[0])){
//                average[i] += stoi(output[i][j]);
//            }
//        }
//    }
//    for (int i =0 ; i< part_size; i++){
//        typecheck.clear();
//        typecheck = output[0][i];
//        if (isdigit(typecheck[0])){
//            average[i] /= output[i].size();
//        }
//    }
//
//    // standard deviation  and z score regularization//
//
//    double standard_deviation;
//    double z_score;
//    double value;
//    string z_score_string;
//
//    for (int i =0 ; i< part_size; i++){
//        for (int j = 0; j < (short)output[i].size(); j++){
//            typecheck.clear();
//            typecheck = output[i][j];
//            if (isdigit(typecheck[0])){
//                value = (double) stoi(output[i][j]) - average[i];
//                standard_deviation = pow(value,2) / (output[i].size() - 1);
//                standard_deviation = pow(standard_deviation,0.5);
//                z_score = (value -average[i])/ standard_deviation;
//
//                z_score_string = to_string(z_score);
//                output[i][j].clear();
//                output[i][j] = z_score_string;
//                z_score_string.clear();
//
//            }
//        }
//    }


    return output;

}




struct node{
    vector <node*> * children;
    vector <bool>  used_attributes;
    vector <sample> set;
    int setsize;
    int used_count;
    string value;
};


/*
struct node * newnode( vector <sample> & s, vector <bool> & u_a, int samplesize, int boolcount){
    struct node * node;
    node->children = NULL;
    node->setsize = samplesize;
    node->used_count = boolcount;
    node->used_attributes;
    node->set = new vector <sample> (samplesize);
    for (int i = 0; i < samplesize; i++){
        (*node->set)[i] = (s[i]);
    }
    for (int i = 0; i < boolcount; i++){
        (*node->used_attributes)[i] = u_a[i];
    }
    return (node);
};


*/


bool is_pure(struct node * n){

    
    int sample_count = n->setsize;
    int counter = 0;
    for (int i = 0; i < sample_count; i++){
        counter += n->set[i].truth_value;
    }
    // if all true or all false return true
    if (counter == sample_count || counter == 0){
        return true;
    }

    return false;

}

double entropy(vector <sample> set, int set_size){
    double entropy = 0;
    double positive = 0;
    double negative = 0;
    for (int i = 0; i < set_size; i++){
        if (set[i].truth_value == 1){
            positive += 1;
        }
        else{
            negative += 1;
        }
    }

    entropy = -1*(positive * log2(positive)) - 1*(negative * log2(negative));

    return entropy;
}



double information_gain(struct node * n,
                        vector <vector < sample> > subset){
    double gain  = 0;

    double current_set_entropy = entropy(n->set, n->setsize);
    double set_size = n->setsize;
    vector <double> subset_sizes(subset.size());
    for (int i = 0; i < (short) subset.size(); i++){
        subset_sizes[i] = subset[i].size();
        // create the ratio for gain equation
        subset_sizes[i] /= set_size;

        // now subtract entropys from current entropy to find gain

        current_set_entropy -= (subset_sizes[i]*entropy(subset[i], subset_sizes[i]));
    }
    // gain is what is left over after drop in entropy after split
    gain = current_set_entropy;

    return gain;
}


int find_max_gain(struct node * n){
    vector <double> gain_set;
    vector <vector <sample> > subsets;
    vector <int> subsetsizes;
    vector <string> current_att_choices;
    vector <sample> temp;

    auto options = options_create(n->set);

    for( int i = 0; i< n->used_attributes.size(); i++){
        if (!n->used_attributes[i]){
            current_att_choices = options[i];
            for (int j = 0; j < (short) current_att_choices.size(); j++){
                subsets.push_back(temp);
            }
            //create the subsets
            for (int j = 0; j < (short) current_att_choices.size(); j++){
                for (int k = 0; k < (short) n->setsize; k++){
                    if (n->set[k].particulars[i] == current_att_choices[j].c_str()){
                        subsets[i].push_back(n->set[k]);
                    }
                }
            }
            gain_set.push_back(information_gain(n,subsets));
        }
        else{
            gain_set.push_back(-100);
        }
        subsets.clear();
        current_att_choices.clear();
    }

    double max_gain = -1;
    int attribute_index = 0;
    for (int i = 0 ; i < (short) gain_set.size(); i++){
        if (max_gain < gain_set[i]){
            max_gain = gain_set[i];
            attribute_index = i;
        }
    }

    return attribute_index;


}


int optionsCount (struct node * n,int index){
    int output = 0;

    auto options = options_create(n->set);
    auto different_children = options[index];

    output = (short) different_children.size();

    return output;


}

struct node * child_setup(struct node * parent, int attribute_index, int children_count){


    auto options = options_create(parent->set);
    auto different_children = options[attribute_index];



    vector <vector <sample> > vec((short)different_children.size());
    vector <sample> temp;


    for (int i = 0; i < parent->setsize; i++) {
        for (int j = 0; j < (short) vec.size(); j++) {

            if (parent->set[i].particulars[attribute_index] == different_children[j]) {
                vec[j].push_back(parent->set[i]);
            }

        }
    }

    vector <bool> unused;
    int usedcount = 0;

    for (int i = 0; i < parent->used_attributes.size(); i++){
        if (parent->used_attributes[i]){
            unused.push_back(true);
            usedcount++;
        }
        else if (i == attribute_index) {
            unused.push_back(true);
            usedcount++;
        }
        else{
            unused.push_back(false);
        }
        i++;
    }



    node * tempchild;

    for(int x = 0; x < children_count; x++){
        //struct node * newnode( vector <sample> s, vector <bool> u_a, int samplesize, int boolcount)
        tempchild = new node;
        tempchild->value = parent->set[x].particulars[attribute_index];
        tempchild->set = vec[x];
        tempchild->used_attributes = unused;
        tempchild->setsize = (short) vec[x].size();
        tempchild->used_count = usedcount;
        tempchild->children = nullptr;
        //newnode(vec[x], unused, (short) vec[x].size(), (short) unused.size());
        //cout << "child truth value of first value of the set: " << (*(*parent->children)[x]->set)[0].truth_value<< endl;


        (*parent->children)[x] = tempchild;

    }


    return parent;


}

struct node * split(struct node * basenode) {


    // base cases //

    if (is_pure(basenode)){
        return basenode;
    }
    if (basenode->used_count == (int) basenode->used_attributes.size()){
        return basenode;
    }




    vector<double> gain_set;
    int attribute_index = find_max_gain(basenode);
    //3. for each value of A, create a new child node
    int childrencount = optionsCount(basenode, attribute_index);

    basenode->children = new vector <node*> (childrencount);
    //4. split training {examples} to child nodes
    basenode = child_setup(basenode, attribute_index, childrencount);
    //5. for each child node / subset
    //if subset is pure, stop
    //else Split (child node, {subset})
    int i = 0;
    while (basenode->children!= NULL) {
        if ( is_pure((*basenode->children)[i]) ) {
            return basenode;
        } else {
            split((*basenode->children)[i]);
        }
        i++;
    }
    return basenode;
}





void printtree (struct node * n){
    cout << "value of node: " << n->value << endl;
    if (n->children == nullptr){
        return;
    }

    for (int i = 0; i < (short) (*n->children).size(); i++ ){
        printtree((*n->children)[i]);
    }
}



#endif //DECISION_TREE_ALGORITHM_MODEL_TREE_H
