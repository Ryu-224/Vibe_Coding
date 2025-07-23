#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 32
#define MAX_KINGS 30

// 왕의 정보를 저장하는 트리 노드 구조체
typedef struct KingNode {
    char name[MAX_NAME_LENGTH];
    struct KingNode* parent;
    struct KingNode* first_child;
    struct KingNode* next_sibling;
} KingNode;

// 함수 프로토타입 선언
KingNode* create_node(const char* name);
KingNode* find_node(KingNode* root, const char* name);
void add_child(KingNode* parent, KingNode* child);
KingNode* build_dynasty_tree(const char* filename);
void free_tree(KingNode* root);
int get_all_kings(KingNode* root, KingNode** list);

// 퀴즈 함수 프로토타입 선언
void print_dynasty_tree(KingNode* root);
void quiz1(KingNode* root);
void quiz2(KingNode* root);
void quiz3(KingNode* root);
void quiz4(KingNode* root);
void quiz5(KingNode* root);
void quiz6(KingNode* root);
void quiz7(KingNode* root);
void quiz8(KingNode* root);
void quiz9(KingNode* root);
void quiz10(KingNode* root);

int main() {

    // 1. 트리 생성
    KingNode* root = build_dynasty_tree("조선왕조.txt");
    if (root == NULL) {
        return 1;
    }

    // 2. 생성된 트리 구조 출력 (이 부분을 추가)
    print_dynasty_tree(root);

    printf("### 조선 왕조 퀴즈 ###\n\n");

    // 3. 각 퀴즈 함수를 순서대로 호출하고 결과 출력
    printf("1. 조선의 왕을 순서대로 출력하시오.\n > ");
    quiz1(root);
    printf("\n");

    printf("2. 조선의 왕을 역순으로 출력하시오.\n > ");
    quiz2(root);
    printf("\n");

    printf("3. 조선의 왕은 모두 몇 명인가?\n > ");
    quiz3(root);
    printf("\n");

    printf("4. 조선의 왕 중에서 인조의 후손은 누구누구인가?\n > ");
    quiz4(root);
    printf("\n");

    printf("5. 직계 후손이 왕이 되지 못한 왕은 누구누구인가?\n > ");
    quiz5(root);
    printf("\n");

    printf("6. 직계 후손이 왕이 된 수가 가장 많은 왕은 누구인가?\n > ");
    quiz6(root);
    printf("\n");

    printf("7. 선조의 형제로 조선의 왕이 된 사람은 누구인가?\n > ");
    quiz7(root);
    printf("\n");

    printf("8. 순종의 직계 선조를 모두 출력하시오.\n > ");
    quiz8(root);
    printf("\n");

    printf("9. 직계 후손이 2명 이상 왕이 된 왕은 몇 명인가?\n > ");
    quiz9(root);
    printf("\n");

    printf("10. 예종은 태종의 몇 대 후손인가?\n > ");
    quiz10(root);
    printf("\n");

    // 4. 트리 메모리 해제
    free_tree(root);

    return 0;
}

//--- 트리 기본 함수들 ---

// 새로운 노드를 생성하고 초기화
KingNode* create_node(const char* name) {
    KingNode* new_node = (KingNode*)malloc(sizeof(KingNode));
    if (new_node == NULL) {
        perror("메모리 할당 실패");
        exit(1);
    }
    strncpy(new_node->name, name, MAX_NAME_LENGTH - 1);
    new_node->name[MAX_NAME_LENGTH - 1] = '\0';
    new_node->parent = NULL;
    new_node->first_child = NULL;
    new_node->next_sibling = NULL;
    return new_node;
}

// 트리에서 특정 이름을 가진 노드를 검색 (전위 순회 방식)
KingNode* find_node(KingNode* current, const char* name) {
    if (current == NULL) {
        return NULL;
    }
    if (strcmp(current->name, name) == 0) {
        return current;
    }
    // 자식 노드에서 검색
    KingNode* found_in_child = find_node(current->first_child, name);
    if (found_in_child != NULL) {
        return found_in_child;
    }
    // 형제 노드에서 검색
    return find_node(current->next_sibling, name);
}

// 부모 노드에 자식 노드를 추가
void add_child(KingNode* parent, KingNode* child) {
    child->parent = parent;
    if (parent->first_child == NULL) {
        parent->first_child = child;
    }
    else {
        KingNode* sibling = parent->first_child;
        while (sibling->next_sibling != NULL) {
            sibling = sibling->next_sibling;
        }
        sibling->next_sibling = child;
    }
}

// 파일로부터 왕조 트리를 생성
KingNode* build_dynasty_tree(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("파일 열기 실패");
        return NULL;
    }

    char king_name[MAX_NAME_LENGTH];
    char parent_name[MAX_NAME_LENGTH];
    KingNode* root = NULL;

    // 첫 줄(태조)을 읽어 루트 노드 생성
    if (fscanf(file, "%s", king_name) == 1) {
        root = create_node(king_name);
    }
    else {
        fclose(file);
        return NULL;
    }

    // 나머지 왕들을 읽어 트리에 추가
    while (fscanf(file, "%s %s", king_name, parent_name) == 2) {
        KingNode* parent_node = find_node(root, parent_name);
        if (parent_node != NULL) {
            KingNode* new_king = create_node(king_name);
            add_child(parent_node, new_king);
        }
    }

    fclose(file);
    return root;
}

// 트리의 모든 노드 메모리를 해제 (후위 순회 방식)
void free_tree(KingNode* current) {
    if (current == NULL) {
        return;
    }
    free_tree(current->first_child);
    free_tree(current->next_sibling);
    free(current);
}


//--- 퀴즈 답변을 위한 헬퍼 함수들 ---

// 전위 순회하며 노드를 배열에 저장
void preorder_traversal(KingNode* node, KingNode** list, int* count) {
    if (node == NULL) {
        return;
    }
    list[*count] = node;
    (*count)++;
    preorder_traversal(node->first_child, list, count);
    preorder_traversal(node->next_sibling, list, count);
}

// 트리의 모든 왕 노드를 순서대로 배열에 담아 반환
int get_all_kings(KingNode* root, KingNode** list) {
    int count = 0;
    preorder_traversal(root, list, &count);
    return count;
}

// 특정 노드의 모든 후손을 재귀적으로 출력
void print_descendants(KingNode* node) {
    if (node == NULL) {
        return;
    }
    KingNode* child = node->first_child;
    while (child != NULL) {
        printf("%s ", child->name);
        print_descendants(child);
        child = child->next_sibling;
    }
}

// 리프 노드(자식이 없는 왕)를 찾는 재귀 함수
void find_kings_without_heirs(KingNode* node) {
    if (node == NULL) {
        return;
    }
    if (node->first_child == NULL) {
        printf("%s ", node->name);
    }
    find_kings_without_heirs(node->first_child);
    find_kings_without_heirs(node->next_sibling);
}

// 노드의 직계 자식 수를 계산
int count_direct_children(KingNode* node) {
    if (node == NULL || node->first_child == NULL) {
        return 0;
    }
    int count = 0;
    KingNode* child = node->first_child;
    while (child != NULL) {
        count++;
        child = child->next_sibling;
    }
    return count;
}


//--- 퀴즈 1~10번 함수 구현 ---

// 1. 조선의 왕을 순서대로 출력
void quiz1(KingNode* root) {
    KingNode* king_list[MAX_KINGS];
    int count = get_all_kings(root, king_list);
    for (int i = 0; i < count; i++) {
        printf("%s ", king_list[i]->name);
    }
    printf("\n");
}

// 2. 조선의 왕을 역순으로 출력
void quiz2(KingNode* root) {
    KingNode* king_list[MAX_KINGS];
    int count = get_all_kings(root, king_list);
    for (int i = count - 1; i >= 0; i--) {
        printf("%s ", king_list[i]->name);
    }
    printf("\n");
}

// 3. 조선의 왕은 모두 몇 명인가?
void quiz3(KingNode* root) {
    KingNode* king_list[MAX_KINGS];
    int count = get_all_kings(root, king_list);
    printf("%d명\n", count);
}

// 4. 인조의 후손은 누구누구인가?
void quiz4(KingNode* root) {
    KingNode* injo = find_node(root, "인조");
    if (injo != NULL) {
        print_descendants(injo);
    }
    printf("\n");
}

// 5. 직계 후손이 왕이 되지 못한 왕은?
void quiz5(KingNode* root) {
    find_kings_without_heirs(root);
    printf("\n");
}

// 6. 직계 후손이 왕이 된 수가 가장 많은 왕은?
void quiz6(KingNode* root) {
    KingNode* king_list[MAX_KINGS];
    int count = get_all_kings(root, king_list);

    int max_children = -1;
    char king_name[MAX_NAME_LENGTH] = "";

    for (int i = 0; i < count; i++) {
        int num_children = count_direct_children(king_list[i]);
        if (num_children > max_children) {
            max_children = num_children;
            strcpy(king_name, king_list[i]->name);
        }
    }
    printf("%s (%d명)\n", king_name, max_children);
}

// 7. 정종의 형제로 조선의 왕이 된 사람은?
void quiz7(KingNode* root) {
    KingNode* jeongjong = find_node(root, "정종");
    if (jeongjong != NULL && jeongjong->parent != NULL) {
        KingNode* parent = jeongjong->parent;
        KingNode* sibling = parent->first_child;
        while (sibling != NULL) {
            if (strcmp(sibling->name, "정종종") != 0) {
                printf("%s ", sibling->name);
            }
            sibling = sibling->next_sibling;
        }
    }
    printf("\n");
}

// 8. 순종의 직계 선조를 모두 출력
void quiz8(KingNode* root) {
    KingNode* sunjong = find_node(root, "순종");
    if (sunjong != NULL) {
        KingNode* current = sunjong->parent;
        while (current != NULL) {
            printf("%s ", current->name);
            current = current->parent;
        }
    }
    printf("\n");
}

// 9. 직계 후손이 2명 이상 왕이 된 왕은 몇 명인가?
void quiz9(KingNode* root) {
    KingNode* king_list[MAX_KINGS];
    int total_kings = get_all_kings(root, king_list);
    int count = 0;

    for (int i = 0; i < total_kings; i++) {
        if (count_direct_children(king_list[i]) >= 2) {
            count++;
        }
    }
    printf("%d명\n", count);
}

// 10. 예종은 태종의 몇 대 후손인가?
void quiz10(KingNode* root) {
    KingNode* yejong = find_node(root, "예종");
    KingNode* taejong = find_node(root, "태종");
    int generations = 0;

    if (yejong && taejong) {
        KingNode* current = yejong;
        while (current != NULL && current != taejong) {
            current = current->parent;
            generations++;
        }
        if (current == taejong) {
            printf("%d대 후손\n", generations);
        }
        else {
            printf("직계 관계가 아닙니다.\n");
        }
    }
}

//--- 트리 구조 시각화 출력 함수 ---

/**
 * @brief 재귀적으로 트리 구조를 출력하는 헬퍼 함수
 * @param node 현재 출력할 노드
 * @param prefix 현재 노드 이름 앞에 붙일 접두사 문자열 (예: "│   ")
 * @param is_last 현재 노드가 형제 노드 중 마지막인지를 나타내는 플래그
 */
void print_tree_recursive(KingNode* node, const char* prefix, int is_last) {
    if (node == NULL) {
        return;
    }

    // 1. 현재 노드 정보 출력 (접두사 + 연결선 + 왕 이름)
    printf("%s", prefix);
    printf("%s", is_last ? "└─ " : "├─ "); // 마지막 자식이면 '└─', 아니면 '├─'
    printf("%s\n", node->name);

    // 2. 자식 노드들에게 전달할 새로운 접두사 문자열을 만듭니다.
    char new_prefix[512]; // 접두사 문자열을 저장할 버퍼
    snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, is_last ? "    " : "│   ");

    // 3. 모든 자식 노드를 순회하며 재귀적으로 함수를 호출합니다.
    KingNode* child = node->first_child;
    while (child != NULL) {
        // 다음 형제 노드가 없는 경우, 현재 자식이 마지막입니다.
        int last_sibling = (child->next_sibling == NULL);
        print_tree_recursive(child, new_prefix, last_sibling);
        child = child->next_sibling;
    }
}

/**
 * @brief 왕조 트리 구조 출력을 시작하는 메인 함수
 * @param root 트리의 루트 노드
 */
void print_dynasty_tree(KingNode* root) {
    if (root == NULL) {
        printf("트리가 비어있습니다.\n");
        return;
    }
    printf("\n--- 조선 왕조 트리 구조 ---\n");
    // 루트 노드를 먼저 출력합니다.
    printf("%s\n", root->name);

    // 루트의 직계 자식들부터 재귀적 출력을 시작합니다.
    KingNode* child = root->first_child;
    while (child != NULL) {
        int is_last = (child->next_sibling == NULL);
        print_tree_recursive(child, "", is_last);
        child = child->next_sibling;
    }
    printf("---------------------------\n\n");
}
