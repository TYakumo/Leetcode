#include <iostream>
#include <vector>
#include <list>
using namespace std;

class Solution {

public:

    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        vector <int> ret;
        list < pair<int, int> > bestValue;

        for (int i = 0; i < nums.size(); i++) {
            while(!bestValue.empty() && bestValue.front().first+k-1 < i) {
                bestValue.pop_front();
            }

            while (!bestValue.empty() && bestValue.back().second <= nums[i]) {
                bestValue.pop_back();
            }

            bestValue.push_back(make_pair(i, nums[i]));

            if (i >= k-1) {
                ret.push_back(bestValue.front().second);
            }
        }

        return ret;
    }

};
