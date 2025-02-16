#include "iostream"
#include <map>
#include <vector>
#include <queue>


struct Node {
    static const int ASIZE = 26;
    int next[ASIZE];
    int link;
    bool term = false;

    Node() {
        for (int i = 0; i < ASIZE; ++i) {
            next[i] = -1;
        }
        link = -1;
        term = false;
    }
};


struct AhoCorasic {
    std::vector<Node> trie;

    AhoCorasic() {
        trie.push_back(Node());
    }

    void add_word(std::string const &s) {
        int cur = 0;
        for (char c: s) {
            c -= 'a';
            if (trie[cur].next[c] == -1) {
                trie.push_back(Node());
                trie[cur].next[c] = trie.size() - 1;
            }
            cur = trie[cur].next[c];
        }
        trie[cur].term = true;
        std::cout << s << std::endl;
    }

    void build() {
        trie[0].link = 0;
        for (int i = 0; i < Node::ASIZE; ++i) {
            if (trie[0].next[i] != -1) {
                continue;
            }
            trie[0].next[i] = 0;
        }
        std::queue<int> q;
        q.push(0);
        while (!q.empty()) {
            int cur = q.front();
            q.pop();
            for (int i = 0; i < Node::ASIZE; ++i) {
                int u = trie[cur].next[i];
                if (trie[cur].link != -1) {
                    continue;
                }
                trie[u].link = u == 0 ? 0 : trie[trie[cur].link].next[i];
                for (int c = 0; c < Node::ASIZE; ++c) {
                    if (trie[u].next[c] != -1) {
                        continue;
                    }
                    trie[u].next[c] = trie[trie[u].link].next[c];
                }
                q.push(0);
            }
        }
        std::cout << "builded" << std::endl;
    }
};

int main() {
    AhoCorasic ac;
    ac.add_word("abc");
    ac.add_word("abcd");
    ac.add_word("hse");
    ac.build();

    return 0;
}
