#include <iostream>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <utility>
#include <climits>
#include <queue>

using namespace std;

const int MAXN = 1e6 + 10;

struct suffix_array {
  int box[MAXN], tp[MAXN], m;
  bool not_equ(int a, int b, int k, int n) {
    return ra[a] != ra[b] || a + k >= n ||
      b + k >= n || ra[a + k] != ra[b + k];
  }
  void radix(int *key, int *it, int *ot, int n) {
    fill_n(box, m, 0);
    for (int i = 0; i < n; ++i) ++box[key[i]];
    partial_sum(box, box + m, box);
    for (int i = n - 1; i >= 0; --i)
      ot[--box[key[it[i]]]] = it[i];
  }
  void make_sa(const string &s, int n) {
    int k = 1;
    for (int i = 0; i < n; ++i) ra[i] = s[i];
    do {
      iota(tp, tp + k, n - k), iota(sa + k, sa + n, 0);
      radix(ra + k, sa + k, tp + k, n - k);
      radix(ra, tp, sa, n);
      tp[sa[0]] = 0, m = 1;
      for (int i = 1; i < n; ++i) {
        m += not_equ(sa[i], sa[i - 1], k, n);
        tp[sa[i]] = m - 1;
      }
      copy_n(tp, n, ra);
      k *= 2;
    } while (k < n && m != n);
  }
  void make_he(const string &s, int n) {
    for (int j = 0, k = 0; j < n; ++j) {
      if (ra[j])
        for (; s[j + k] == s[sa[ra[j] - 1] + k]; ++k)
          ;
      he[ra[j]] = k, k = max(0, k - 1);
    }
  }
  int sa[MAXN], ra[MAXN], he[MAXN], n;
  string s;
  void build(const string &ss) {
    s = ss;
    n = s.length();
    fill_n(sa, n, 0), fill_n(ra, n, 0), fill_n(he, n, 0);
    fill_n(box, n, 0), fill_n(tp, n, 0), m = 256;
    make_sa(s, n), make_he(s, n);
    for (int i = 0; i < n; i++) {
      // cout << s.substr(sa[i], n - sa[i]) << endl;
    }
  }
} SA;

struct match{
  int leftmin;
  int rightmin;
  int leftmax;
  int rightmax;
  int pstart;
  int pend;
  int start;
  int end;
  int length;
  int remain;
  int segment_id;
  int internal;
};

int check (const string &p, int id) {
  for (int i = 0, j = min((int)p.length(), SA.n - id); i < j; i ++) {
    if (p[i] < SA.s[id + i]) {
      return 0;
    }
    else if (p[i] > SA.s[id + i]) {
      return 2;
    }
  }
  if (p.length() > SA.n - id) {
    return 2;
  }
  return 1;
}

int binary_search(const string &p, int l, int r, int mode) {
  if (r == l) {
    return l;
  }
  if (r == l + 1) {
    int ret = check(p, SA.sa[l]);
    // cout << p << " " << l << " " << r << " " << ret << endl;
    if (ret - mode < 1) {
      return l;
    }
    else {
      return r;
    }
  }
  else {
    int mid = (l + r) >> 1;
    int ret = check(p, SA.sa[mid]);
    // cout << p << " " << l << " " << r << " " << ret << endl;
    if (ret - mode < 1) {
      return binary_search(p, l, mid, mode);
    }
    else {
      return binary_search(p, mid, r, mode);
    }
  }
}

pair<int, int> find_occ (const string &p) {
  int l = binary_search(p, 0, SA.n - 1, 1);
  int r = binary_search(p, l, SA.n - 1, 0);
  return make_pair(l, r);
}

vector<match> find_in_suffix_array (const string &p, const int offset, const int length) {
  vector<match> N;
  pair<int, int> occ = find_occ(p);
  // cout << p << " " << occ.first << " " << occ.second << endl;
  // cout << "p = " << p << endl;
  for (int i = occ.first; i < occ.second; i ++) {
    match m;
    // cout << SA.s.substr(SA.sa[i], p.length()) << endl;
    m.start = SA.sa[i]; // m.start = min(offset, SA.sa[i]); // m.start = i.segment_start;
    m.end = m.start + p.length() - 1; // m.end = i.segment_end;
    m.length = p.length(); // m.length = i.segment_length;
    m.remain = length - p.length() - offset;// m.remain = m.length - m.end - 1; // m.remain = m.length - m.end;
    // m.segment_id = SA.sa[i] - m.start; // m.segment_id = i.segment_id;
    m.internal = 0;
    N.push_back(m);
  }
  return N;
}

vector<match> find_match (const string &p, const int ceiling_cost) {
  vector<match> M;
  for (int start = 0; start < p.length(); start ++) {
    for (int end = start; end < p.length(); end ++) {
      int remain = p.length() - end - 1;
      vector<match> ret = find_in_suffix_array(p.substr(start, end - start + 1), start, p.length());
      // cout << p.substr(start, end - start + 1) << " " << ret.size() << endl;
      for (auto &m:ret) {
        // cout << m.start << " " << m.end << endl;
      }
      if (ret.empty()) {
        break;
      }
      for (auto &m:ret) {
        /*m.leftmin = m.start - start;
        if (m.leftmin == 0 && start > 0) {
          m.leftmin = 1;
        }
        m.rightmin = m.remain - remain;
        if (m.rightmin == 0 && remain > 0) {
          m.rightmin = 1;
        }
        min_cost = m.leftmin + m.rightmin;
        if (min_cost > ceiling_cost) {
          break;
        }
        m.leftmax = max(m.start, start);
        m.rightmax = max(m.remain, remain);*/
        m.leftmin = min(start, 1);
        m.rightmin = min(remain, 1);
        m.pstart = start;
        m.pend = end;
        M.push_back(m);
      }
    }
  }
  return M;
}

struct agenda {
  match m1;
  match m2;
  int internal;
  int mincost;
  int priority;
};

bool check_combinable (const match &m1, const match &m2) {
  if (m1.end >= m2.start) {
    return false;
  }
  if (m1.pend >= m2.pstart) {
    return false;
  }
  return true;
}

agenda combinable (const match &m1, const match &m2) {
  agenda a;
  a.m1 = m1;
  a.m2 = m2;
  // int delete = m2.start - m1.end - 1;
  // int insert = m2.pstart - m1.pend - 1;
  int internal = max(m2.pstart - m1.pend - 1, m2.start - m1.end - 1); // int internal = max(insert, delete);
  a.internal = internal;
  a.mincost = m1.leftmin + m2.rightmin + internal;
  a.priority = -a.mincost;
  return a;
}

struct agenda_cmp {
    bool operator () (const agenda &x, const agenda &y) {
        return x.priority < y.priority;
    }
};

int parse_validate (vector<match> &M, int ceiling_cost) {
  priority_queue<agenda, vector<agenda>, agenda_cmp> A;
  for (auto &m1:M) {
    for (auto &m2:M) {
      if (check_combinable(m1, m2)) {
        A.push(combinable(m1, m2));
      }
    }
  }
  int cost = INT_MAX;
  for (auto &m:M) {
    cost = min(cost, m.leftmax + m.rightmax);
  }
  // cout << "cost = " << cost << endl;
  while (!A.empty()) {
    agenda a = A.top();
    A.pop();
    if (a.mincost > ceiling_cost) {
      break;
    }
    match mm;
    mm.leftmin = a.m1.leftmin;
    mm.leftmax = a.m1.leftmax;
    mm.rightmin = a.m2.rightmin;
    mm.rightmax = a.m2.rightmax;
    mm.start = a.m1.start;
    mm.end = a.m2.end;
    mm.pstart = a.m1.pstart;
    mm.pend = a.m2.pend;
    mm.internal = a.m1.internal + a.m2.internal + a.internal;
    cost = min(cost , mm.leftmax + mm.rightmax + mm.internal);
    for (auto &m:M) {
      if (check_combinable(mm, m)) {
        A.push(combinable(mm, m));
      }
    }
  }
  return cost;
}

struct segment{
  int begin;
  int end;
  vector<match> M;
  int sumlength;
  int priority;
  int length;
};

struct segment_cmp {
    bool operator () (const segment &x, const segment &y) {
        return x.priority < y.priority;
    }
};

vector<pair<int, int>> find_segments (const string &p, const vector<match> &M, int ceiling_cost) {
  priority_queue<segment, vector<segment>, segment_cmp> A;
  for (int start = 0; start < SA.n; start ++) {
    for (int end = max(start, start + (int)p.length() - 1 - ceiling_cost); end < SA.n && end <= start + (int)p.length() - 1 + ceiling_cost; end ++) {
      segment s;
      s.sumlength = 0;
      // cout << "segment " << start << " " << end << " " << SA.s.substr(start, end - start + 1) << endl;
      for (auto m:M) {
        if (m.start >= start && m.end <= end) {
          // cout << m.start << " " << m.end << endl;
          m.leftmin = max(m.leftmin, abs(m.start - start));
          m.rightmin = max(m.rightmin, abs(m.remain - ((int)p.length() - m.pend - 1)));
          m.leftmax = max(m.start - start, m.pstart);
          m.rightmax = max(end - m.end, m.remain);
          // cout << p.substr(m.pstart, m.pend - m.pstart + 1) << " " << SA.s.substr(m.start, m.end - m.start + 1) << " " << m.start << " " << m.end << " " << m.leftmax << " " << m.rightmax << endl;
          s.M.push_back(m);
          s.sumlength += m.length;
        }
      }
      if (s.sumlength == 0) {
        break;
      }
      // cout << start << " " << end << " " << s.sumlength << endl;
      s.begin = start;
      s.end = end;
      s.length = end - start + 1;
      s.priority = s.sumlength;
      A.push(s);
    }
  }
  vector<pair<int, int>> S;
  while (!A.empty()) {
    segment a = A.top();
    A.pop();
    // cout << a.begin << " " << a.end << " " << SA.s.substr(a.begin, a.end - a.begin + 1) << endl;
    if (max(a.length, (int)p.length()) - a.sumlength > ceiling_cost) {
      // cout << max(a.length, (int)p.length()) << " " << a.sumlength << " " << ceiling_cost << endl;
      break;
    }
    int cost = parse_validate(a.M, ceiling_cost);
    if (cost < ceiling_cost) {
      ceiling_cost = cost;
      S.resize(0);
    }
    if (cost == ceiling_cost) {
      S.push_back(make_pair(a.begin, a.end));
    }
  }
  // cout << "ceiling_cost = " << ceiling_cost << endl;
  return S;
}

signed main () {
  string s;
  cin >> s;
  SA.build(s);
  string p;
  cin >> p;
  int ceiling_cost = ceil(0.3 * p.length());
  // cout << ceiling_cost << endl;
  vector<match> M;
  vector<pair<int, int>> S;
  M = find_match(p, ceiling_cost);
  // cout << M.size() << endl;
  for (auto &m:M) {
    // cout << m.start << " " << m.end << endl;
  }
  S = find_segments(p, M, ceiling_cost);
  // cout << S.size() << endl;
  for (auto &s:S) {
    cout << SA.s.substr(s.first, s.second - s.first + 1) << endl;
  }
}
