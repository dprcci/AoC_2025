#include <vector>
#include <fstream>
#include <iostream>
#include <set>
#include <unordered_map>
#include <stack>

using namespace std;

tuple<vector<short>, size_t, size_t> read_input(string& filename){
    ifstream f(filename);
    if (!f.is_open()) {
        cerr << "Error opening the file!";
        return {};
    }
    vector<short> floor_plan{};
    string s;
    size_t row_lentgh = 0;
    size_t col_length = 0;
    while (getline(f, s)){
        row_lentgh =  s.size();
        cout << s << endl;
        for (auto& c : s){
            floor_plan.push_back(c == '@' ? 1 : 0);
        }
        col_length += 1;
    }
    f.close();
    return {floor_plan, row_lentgh, col_length};
}

int find_accessible_rolls(vector<short> floorplan, size_t row_size, size_t col_size, size_t max_neighbors_allowed = 4){
    int result = 0;
    for (size_t i = 0; i < floorplan.size(); i++){
        if (floorplan[i] == 0) continue;

        size_t row_idx = i / row_size;
        size_t col_idx = i % row_size;
        size_t neighbors_sum = 0;
        
        // row idx 0 -> col_size
        // col_idx 0 -> row_size
        int x_lower_bound = row_idx == 0 ? 0 : -1;
        int x_upper_bound = (row_idx + 1 == col_size) ? 0 : 1;
        int y_lower_bound = col_idx % row_size == 0 ? 0 : -1;
        int y_upper_bound = col_idx + 1 == row_size ? 0 : 1;

        for (int x = x_lower_bound; x <= x_upper_bound; x++){
            for (int y = y_lower_bound; y <= y_upper_bound; y++){
                size_t neighbor_idx = col_idx + y + ((row_idx + x) * row_size);
                if (neighbor_idx == i) continue;
                
                cout << neighbor_idx << " " << x << " " << y << endl;
                neighbors_sum += floorplan[neighbor_idx];
            }
        }
        result += (neighbors_sum < max_neighbors_allowed) ? 1 : 0;
    }
    return result;
}

tuple<stack<size_t>, unordered_map<size_t, tuple<size_t,vector<size_t>>>> build_starting_plan(vector<short> floorplan, size_t row_size, size_t col_size, size_t max_neighbors_allowed = 4){
    unordered_map<size_t, tuple<size_t,vector<size_t>>> neighbors_indices{};
    stack<size_t> initially_removable{};
    for (size_t i = 0; i < floorplan.size(); i++){
        if (floorplan[i] == 0) continue;

        size_t row_idx = i / col_size;
        size_t col_idx = i % row_size;
        size_t neighbors_sum = 0;
        
        // row idx 0 -> col_size
        // col_idx 0 -> row_size
        int x_lower_bound = row_idx == 0 ? 0 : -1;
        int x_upper_bound = (row_idx + 1 == col_size) ? 0 : 1;  
        int y_lower_bound = col_idx == 0 ? 0 : -1; 
        int y_upper_bound = (col_idx + 1 == row_size) ? 0 : 1; 

        vector<size_t> neighbor_idx_list;
        for (int x = x_lower_bound; x <= x_upper_bound; x++){
            for (int y = y_lower_bound; y <= y_upper_bound; y++){
                size_t neighbor_row = row_idx + x;
                size_t neighbor_col = col_idx + y;
                size_t neighbor_idx = neighbor_row * row_size + neighbor_col;  
                if (neighbor_idx == i) continue;
                
                neighbors_sum += floorplan[neighbor_idx];
                neighbor_idx_list.push_back(neighbor_idx);
            }
        }
        neighbors_indices[i] = {neighbors_sum, neighbor_idx_list};
        if(neighbors_sum < max_neighbors_allowed){
            initially_removable.push(i);
        }
    }
    return {initially_removable, neighbors_indices};
}


int forklitft_removal(vector<short> floorplan, size_t row_size, size_t col_size, size_t max_neighbors_allowed = 4){
    auto inital_params = build_starting_plan(floorplan, row_size, col_size, max_neighbors_allowed); 
    set<size_t> visited{};
    stack<size_t> removable = get<0>(inital_params);  
    unordered_map<size_t, tuple<size_t,vector<size_t>>> floor_data = get<1>(inital_params);
    
    int removed = 0;
    while(!removable.empty()) {
        size_t current_cell_idx = removable.top(); 
        removable.pop();
        if (visited.find(current_cell_idx) != visited.cend()) continue;

        for (auto& neighbor_idx : get<1>(floor_data[current_cell_idx])){
            size_t updated_neighbors_count = --(get<0>(floor_data[neighbor_idx]));
            if(updated_neighbors_count < max_neighbors_allowed){
                removable.push(neighbor_idx);
            }
        }
        visited.insert(current_cell_idx);
        removed += 1;
    }
    
    return removed;
}


int main(int argc, char* argv[]){
    string file_name = argc > 1 ? argv[1] : "example.txt";
    tuple<vector<short>, int, int> parse_floor_plan = read_input(file_name);
    vector<short> floorplan = get<0>(parse_floor_plan);
    size_t row_size = get<1>(parse_floor_plan);
    size_t col_size =get<2>(parse_floor_plan);
    //int result = find_accessible_rolls(floorplan, row_size, col_size);
    //cout << "Found "<< result << " rolls accessible." << endl;
    int result = forklitft_removal(floorplan, row_size, col_size);
    cout << "Found "<< result << " removable rolls." << endl;

}