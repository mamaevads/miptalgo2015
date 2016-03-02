#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <cassert>
#include <memory>

using namespace std;

const int NONE_ID = -2;
const int SERVER_ID = -1;

struct TMessage
{
	TMessage(int from, int to) : from(from), to(to) {}
	int from, to;
	virtual ~TMessage() {}
};

struct TMessageSimple : public TMessage
{
	TMessageSimple(const string& data, int from, int to) : data(data), TMessage(from, to) {}
	string data;
	virtual ~TMessageSimple() {}
};

vector<string> split(const string& text, char delimiter = '\t') {
	vector<string> res;
	string word;
	for (char c : text)
		if (c == delimiter){
			res.push_back(word);
			word = "";
		}
		else {
			word += c;
		}
		return res;
}

class TNode
{
public:
	virtual vector<shared_ptr<TMessage>> receive(TMessage& message) = 0;
	virtual void addConnection(int to) = 0;
	virtual ~TNode() {}
private:
};

// naive implementation to test
class TNaive : public TNode
{
public:
	TNaive(int id) : id(id) {}
	virtual vector<shared_ptr<TMessage>> receive(TMessage& message) override {
		assert(message.to == id);
		if (message.from != SERVER_ID) {
			TMessageSimple& messageSimple = dynamic_cast<TMessageSimple&>(message);
			cerr << "received from vertice #" << messageSimple.from << " in vertice #" << messageSimple.to << " information " << messageSimple.data << endl;
		}
		else
			cerr << "received from server in vertice #" << id << endl;
		vector<shared_ptr<TMessage>> res;
		if (!neighbors.empty() && (rand() & 1)) {
			int to = neighbors[rand() % neighbors.size()];
			cerr << id << " want to send message to " << to << endl;
			res.push_back(move(shared_ptr<TMessage>(new TMessageSimple("interval_message", id, to))));
		}
		return res;
	}
	virtual void addConnection(int to) override {
		assert(find(neighbors.begin(), neighbors.end(), to) == neighbors.end());
		neighbors.push_back(to);
	}
	virtual ~TNaive() { cerr << "!" << endl; }
private:
	int id;
	vector<int> neighbors;
};

//dispatcher of tasks
class TTasker
{
public:
	TTasker(int n) : queueMessages(n)
	{}
	void add(shared_ptr<TMessage>& message) {
		queueMessages[message->from].push(move(message));
	}
	// get message from one of queues according to sizes, simple implementation (in the future with segment tree)
	shared_ptr<TMessage> get() {
		int allMessages = 0;
		for (auto q : queueMessages)
			allMessages += q.size();
		if (!allMessages)
			return shared_ptr<TMessage>(new TMessage(NONE_ID, NONE_ID));

		int num = rand() % allMessages;

		for (auto& q : queueMessages){
			num -= q.size();
			if (num < 0) {
				auto res(move(q.front()));
				q.pop();
				return res;
			}
		}
		assert(false);
	}
private:
	vector<queue<shared_ptr<TMessage>>> queueMessages;

};

//server - object that coordinate all communication between nodes. 
class TServer
{
public:
	TServer(vector<shared_ptr<TNode>>&& nodes) : tasker(nodes.size()), nodes(move(nodes)) {
		initRandomSubset();
	}
	void start() {
		while (true) {
			auto cur = tasker.get();
			if (cur->to == NONE_ID)
				break;
			auto received = nodes[cur->to]->receive(*cur);
			for (auto& message : received)
				tasker.add(message);
		}
	}

private:
	void initRandomSubset(double part = 0.2) {
		vector<int> vertices;
		for (int v = 0; v < nodes.size(); v++)
			vertices.push_back(v);
		random_shuffle(vertices.begin(), vertices.end());
		int selected = max(int(part * nodes.size()), 1);
		for (int i = 0; i < selected; i++) {
			int to = vertices[i];
			TMessage wakeUpNigga(SERVER_ID, to);
			auto received = nodes[to]->receive(wakeUpNigga);
			for (auto& message : received)
				tasker.add(message);
		}
	}

	TTasker tasker;
	vector<shared_ptr<TNode>> nodes;
};

int main() {
	freopen("input", "r", stdin);
	int n, m;
	//n vertices with numbers from 0 to n - 1, m edges
	cin >> n >> m;
	vector<shared_ptr<TNode>> nodes(n);
	for (int i = 0; i < n; i++)
		nodes[i] = shared_ptr<TNode>(new TNaive(i));
	for (int i = 0; i < m; i++) {
		int u, v;
		cin >> u >> v;
		nodes[u]->addConnection(v);
		nodes[v]->addConnection(u);
	}
	TServer server(move(nodes));
	server.start();
	return 0;
}

