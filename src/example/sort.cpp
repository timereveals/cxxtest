#include <iostream>

void quick_sort(int array[], const int &i, const int &j) {
  if (j <= i)
    return;
  int pi = i, pj = j;
  int mid = array[j];
  while (pi < pj) {
    while (pi < pj && array[pi] < mid)
      ++pi;
    array[pj] = array[pi];
    while (pi < pj && mid <= array[pj])
      --pj;
    array[pi] = array[pj];
  }
  array[pi] = mid;
  quick_sort(array, i, pi - 1);
  quick_sort(array, pi + 1, j);
}

void heap_sort(int array[], const int& i, const int& j){
  if(j <= i)
    return;
  int p = (i+j)/2;
  while(i <= p){
    if(2*p+1 <= j && array[p] < array[2*p+1]){
      int temp = array[p];
      array[p] = array[2*p+1];
      array[2*p+1] = temp;
    }
    if(2*p+2 <= j && array[p] < array[2*p+2]){
      int temp = array[p];
      array[p] = array[2*p+2];
      array[2*p+2] = temp;
    }
    p--;
  }
  int temp = array[i];
  array[i] = array[j];
  array[j] = temp;
  heap_sort(array, i, j-1);
}

void merge_sort(int array[], const int& i, const int& j){
  if(j <= i)
    return;
  int mid = (j+i+1)/2;
  merge_sort(array, i, mid-1);
  merge_sort(array, mid, j);
  int * temp = new int[j-i+1];
  int * p= temp;
  int a = i, b=mid;
  while(a<mid && b <= j){
    if(array[a]<array[b]){
      *p = array[a];
      ++a;
      ++p;
    }else{
      *p = array[b];
      ++b;
      ++p;
    }
  }
  if(a<mid){
    *p = array[a];
    ++a;
    ++p;
  }
  if(b <=j){
    *p = array[b];
    ++b;
    ++p;
  }
  for(int x=0;x<j-i+1;++x){
    std::cout<<*(temp+x)<<" ";
  }
  std::cout<<"\n";
  --p;
  for(int n= j; i<=n; --n, --p){
    array[n] = *p;
  }

  delete[] temp;

}

int main(int argc, char *argv[]) {
  int a[] = {1, 8, 3, 2, 4, 4, 7, 6, 3, 9, 5, 5};
  int size = sizeof(a) / sizeof(int);

  // quick_sort(a, 0, size - 1);
  // heap_sort(a, 0, size - 1);
  merge_sort(a, 0, size - 1);

  for (int i = 0; i < size; ++i)
    std::cout << a[i] <<" ";
  std::cout<<"\n";

  return 0;
}
