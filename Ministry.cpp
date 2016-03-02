#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

void input(size_t n, size_t m, vector<vector<int> >& building){
	for (size_t floor = 1; floor < n + 1; floor++){
		for (size_t room = 1; room < m + 1; room++){
			cin >> building[floor][room];
		}
	}
}

vector<int> build(size_t n, size_t m, const vector<vector<int> >& building){

	vector<vector<int> > previous(n + 1, vector<int>(m + 1));
	vector<vector<long long> > way(n + 1, vector<long long>(m + 1));

	for (size_t room = 1; room < m + 1; room++){
		way[1][room] = building[1][room];
		previous[1][room] = room;
	}

	for (size_t floor = 2; floor < n + 1; floor++){
		way[floor][1] = way[floor - 1][1] + building[floor][1];
		previous[floor][1] = 1;
		for (size_t room = 2; room < m + 1; room++){
			if (way[floor - 1][room] < way[floor][room - 1]){
				way[floor][room] = way[floor - 1][room] + building[floor][room];
				previous[floor][room] = room;
			}
			else{
				way[floor][room] = way[floor][room - 1] + building[floor][room];
				previous[floor][room] = room - 1;
			}

		}
		for (size_t room = m - 1; room > 0; room--){
			if (way[floor][room] > way[floor][room + 1] + building[floor][room]){
				way[floor][room] = way[floor][room + 1] + building[floor][room];
				previous[floor][room] = room + 1;
			}
		}
	}

	long long min = way[n][1];
	size_t minindex = 1;
	for (size_t room = 1; room < m + 1; room++){
		if (way[n][room] < min){
			min = way[n][room];
			minindex = room;
		}
	}
	size_t prevroom = minindex;
	size_t order = n, counter = 0;

	vector<int> answer;
	answer.push_back(prevroom);

	while (order != 1){
		if (previous[order][prevroom] == prevroom){
			order--;
			answer.push_back(prevroom);
		}
		else{
			prevroom = previous[order][prevroom];
			answer.push_back(prevroom);

		}
		counter++;
	}
	return answer;
}
void output(const vector<int>& answer){
	for (ptrdiff_t k = answer.size() - 1; k >= 0; k--){
		cout << answer[k] << " ";
		}
}

int main(){
	size_t n, m;
	cin >> n >> m;

	vector<vector<int> > building(n + 1, vector<int>(m + 1));
	
	input(n, m, building);
	output(build(n, m, building));
}