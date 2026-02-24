#include <iostream>

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Функция слияния
ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
    ListNode dummy(0);
    ListNode* tail = &dummy;

    while (l1 && l2) {
        if (l1->val <= l2->val) {
            tail->next = l1;
            l1 = l1->next;
        } else {
            tail->next = l2;
            l2 = l2->next;
        }
        tail = tail->next;
    }

    tail->next = l1 ? l1 : l2;

    return dummy.next;
}

// Вспомогательная функция для печати списка
void printList(ListNode* head) {
    while (head) {
        std::cout << head->val << " -> ";
        head = head->next;
    }
    std::cout << "nullptr" << std::endl;
}

// Вспомогательная функция для создания списка
ListNode* createList(int arr[], int n) {
    if (n == 0) return nullptr;
    ListNode* head = new ListNode(arr[0]);
    ListNode* cur = head;
    for (int i = 1; i < n; i++) {
        cur->next = new ListNode(arr[i]);
        cur = cur->next;
    }
    return head;
}

int main() {
    int arr1[] = {1, 2, 4}; // список элементов
    int arr2[] = {1, 3, 4};
    
    ListNode* l1 = createList(arr1, 3);
    ListNode* l2 = createList(arr2, 3);
    
    std::cout << "List 1: ";
    printList(l1);
    std::cout << "List 2: ";
    printList(l2);
    
    ListNode* merged = mergeTwoLists(l1, l2);
    std::cout << "Merged: ";
    printList(merged);
    
    return 0;

}
