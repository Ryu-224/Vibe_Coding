#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 32
#define MAX_KINGS 30

// ���� ������ �����ϴ� Ʈ�� ��� ����ü
typedef struct KingNode {
    char name[MAX_NAME_LENGTH];
    struct KingNode* parent;
    struct KingNode* first_child;
    struct KingNode* next_sibling;
} KingNode;

// �Լ� ������Ÿ�� ����
KingNode* create_node(const char* name);
KingNode* find_node(KingNode* root, const char* name);
void add_child(KingNode* parent, KingNode* child);
KingNode* build_dynasty_tree(const char* filename);
void free_tree(KingNode* root);
int get_all_kings(KingNode* root, KingNode** list);

// ���� �Լ� ������Ÿ�� ����
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

    // 1. Ʈ�� ����
    KingNode* root = build_dynasty_tree("��������.txt");
    if (root == NULL) {
        return 1;
    }

    // 2. ������ Ʈ�� ���� ��� (�� �κ��� �߰�)
    print_dynasty_tree(root);

    printf("### ���� ���� ���� ###\n\n");

    // 3. �� ���� �Լ��� ������� ȣ���ϰ� ��� ���
    printf("1. ������ ���� ������� ����Ͻÿ�.\n > ");
    quiz1(root);
    printf("\n");

    printf("2. ������ ���� �������� ����Ͻÿ�.\n > ");
    quiz2(root);
    printf("\n");

    printf("3. ������ ���� ��� �� ���ΰ�?\n > ");
    quiz3(root);
    printf("\n");

    printf("4. ������ �� �߿��� ������ �ļ��� ���������ΰ�?\n > ");
    quiz4(root);
    printf("\n");

    printf("5. ���� �ļ��� ���� ���� ���� ���� ���������ΰ�?\n > ");
    quiz5(root);
    printf("\n");

    printf("6. ���� �ļ��� ���� �� ���� ���� ���� ���� �����ΰ�?\n > ");
    quiz6(root);
    printf("\n");

    printf("7. ������ ������ ������ ���� �� ����� �����ΰ�?\n > ");
    quiz7(root);
    printf("\n");

    printf("8. ������ ���� ������ ��� ����Ͻÿ�.\n > ");
    quiz8(root);
    printf("\n");

    printf("9. ���� �ļ��� 2�� �̻� ���� �� ���� �� ���ΰ�?\n > ");
    quiz9(root);
    printf("\n");

    printf("10. ������ ������ �� �� �ļ��ΰ�?\n > ");
    quiz10(root);
    printf("\n");

    // 4. Ʈ�� �޸� ����
    free_tree(root);

    return 0;
}

//--- Ʈ�� �⺻ �Լ��� ---

// ���ο� ��带 �����ϰ� �ʱ�ȭ
KingNode* create_node(const char* name) {
    KingNode* new_node = (KingNode*)malloc(sizeof(KingNode));
    if (new_node == NULL) {
        perror("�޸� �Ҵ� ����");
        exit(1);
    }
    strncpy(new_node->name, name, MAX_NAME_LENGTH - 1);
    new_node->name[MAX_NAME_LENGTH - 1] = '\0';
    new_node->parent = NULL;
    new_node->first_child = NULL;
    new_node->next_sibling = NULL;
    return new_node;
}

// Ʈ������ Ư�� �̸��� ���� ��带 �˻� (���� ��ȸ ���)
KingNode* find_node(KingNode* current, const char* name) {
    if (current == NULL) {
        return NULL;
    }
    if (strcmp(current->name, name) == 0) {
        return current;
    }
    // �ڽ� ��忡�� �˻�
    KingNode* found_in_child = find_node(current->first_child, name);
    if (found_in_child != NULL) {
        return found_in_child;
    }
    // ���� ��忡�� �˻�
    return find_node(current->next_sibling, name);
}

// �θ� ��忡 �ڽ� ��带 �߰�
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

// ���Ϸκ��� ���� Ʈ���� ����
KingNode* build_dynasty_tree(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("���� ���� ����");
        return NULL;
    }

    char king_name[MAX_NAME_LENGTH];
    char parent_name[MAX_NAME_LENGTH];
    KingNode* root = NULL;

    // ù ��(����)�� �о� ��Ʈ ��� ����
    if (fscanf(file, "%s", king_name) == 1) {
        root = create_node(king_name);
    }
    else {
        fclose(file);
        return NULL;
    }

    // ������ �յ��� �о� Ʈ���� �߰�
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

// Ʈ���� ��� ��� �޸𸮸� ���� (���� ��ȸ ���)
void free_tree(KingNode* current) {
    if (current == NULL) {
        return;
    }
    free_tree(current->first_child);
    free_tree(current->next_sibling);
    free(current);
}


//--- ���� �亯�� ���� ���� �Լ��� ---

// ���� ��ȸ�ϸ� ��带 �迭�� ����
void preorder_traversal(KingNode* node, KingNode** list, int* count) {
    if (node == NULL) {
        return;
    }
    list[*count] = node;
    (*count)++;
    preorder_traversal(node->first_child, list, count);
    preorder_traversal(node->next_sibling, list, count);
}

// Ʈ���� ��� �� ��带 ������� �迭�� ��� ��ȯ
int get_all_kings(KingNode* root, KingNode** list) {
    int count = 0;
    preorder_traversal(root, list, &count);
    return count;
}

// Ư�� ����� ��� �ļ��� ��������� ���
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

// ���� ���(�ڽ��� ���� ��)�� ã�� ��� �Լ�
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

// ����� ���� �ڽ� ���� ���
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


//--- ���� 1~10�� �Լ� ���� ---

// 1. ������ ���� ������� ���
void quiz1(KingNode* root) {
    KingNode* king_list[MAX_KINGS];
    int count = get_all_kings(root, king_list);
    for (int i = 0; i < count; i++) {
        printf("%s ", king_list[i]->name);
    }
    printf("\n");
}

// 2. ������ ���� �������� ���
void quiz2(KingNode* root) {
    KingNode* king_list[MAX_KINGS];
    int count = get_all_kings(root, king_list);
    for (int i = count - 1; i >= 0; i--) {
        printf("%s ", king_list[i]->name);
    }
    printf("\n");
}

// 3. ������ ���� ��� �� ���ΰ�?
void quiz3(KingNode* root) {
    KingNode* king_list[MAX_KINGS];
    int count = get_all_kings(root, king_list);
    printf("%d��\n", count);
}

// 4. ������ �ļ��� ���������ΰ�?
void quiz4(KingNode* root) {
    KingNode* injo = find_node(root, "����");
    if (injo != NULL) {
        print_descendants(injo);
    }
    printf("\n");
}

// 5. ���� �ļ��� ���� ���� ���� ����?
void quiz5(KingNode* root) {
    find_kings_without_heirs(root);
    printf("\n");
}

// 6. ���� �ļ��� ���� �� ���� ���� ���� ����?
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
    printf("%s (%d��)\n", king_name, max_children);
}

// 7. ������ ������ ������ ���� �� �����?
void quiz7(KingNode* root) {
    KingNode* jeongjong = find_node(root, "����");
    if (jeongjong != NULL && jeongjong->parent != NULL) {
        KingNode* parent = jeongjong->parent;
        KingNode* sibling = parent->first_child;
        while (sibling != NULL) {
            if (strcmp(sibling->name, "����") != 0) {
                printf("%s ", sibling->name);
            }
            sibling = sibling->next_sibling;
        }
    }
    printf("\n");
}

// 8. ������ ���� ������ ��� ���
void quiz8(KingNode* root) {
    KingNode* sunjong = find_node(root, "����");
    if (sunjong != NULL) {
        KingNode* current = sunjong->parent;
        while (current != NULL) {
            printf("%s ", current->name);
            current = current->parent;
        }
    }
    printf("\n");
}

// 9. ���� �ļ��� 2�� �̻� ���� �� ���� �� ���ΰ�?
void quiz9(KingNode* root) {
    KingNode* king_list[MAX_KINGS];
    int total_kings = get_all_kings(root, king_list);
    int count = 0;

    for (int i = 0; i < total_kings; i++) {
        if (count_direct_children(king_list[i]) >= 2) {
            count++;
        }
    }
    printf("%d��\n", count);
}

// 10. ������ ������ �� �� �ļ��ΰ�?
void quiz10(KingNode* root) {
    KingNode* yejong = find_node(root, "����");
    KingNode* taejong = find_node(root, "����");
    int generations = 0;

    if (yejong && taejong) {
        KingNode* current = yejong;
        while (current != NULL && current != taejong) {
            current = current->parent;
            generations++;
        }
        if (current == taejong) {
            printf("%d�� �ļ�\n", generations);
        }
        else {
            printf("���� ���谡 �ƴմϴ�.\n");
        }
    }
}

//--- Ʈ�� ���� �ð�ȭ ��� �Լ� ---

/**
 * @brief ��������� Ʈ�� ������ ����ϴ� ���� �Լ�
 * @param node ���� ����� ���
 * @param prefix ���� ��� �̸� �տ� ���� ���λ� ���ڿ� (��: "��   ")
 * @param is_last ���� ��尡 ���� ��� �� ������������ ��Ÿ���� �÷���
 */
void print_tree_recursive(KingNode* node, const char* prefix, int is_last) {
    if (node == NULL) {
        return;
    }

    // 1. ���� ��� ���� ��� (���λ� + ���ἱ + �� �̸�)
    printf("%s", prefix);
    printf("%s", is_last ? "���� " : "���� "); // ������ �ڽ��̸� '����', �ƴϸ� '����'
    printf("%s\n", node->name);

    // 2. �ڽ� ���鿡�� ������ ���ο� ���λ� ���ڿ��� ����ϴ�.
    char new_prefix[512]; // ���λ� ���ڿ��� ������ ����
    snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, is_last ? "    " : "��   ");

    // 3. ��� �ڽ� ��带 ��ȸ�ϸ� ��������� �Լ��� ȣ���մϴ�.
    KingNode* child = node->first_child;
    while (child != NULL) {
        // ���� ���� ��尡 ���� ���, ���� �ڽ��� �������Դϴ�.
        int last_sibling = (child->next_sibling == NULL);
        print_tree_recursive(child, new_prefix, last_sibling);
        child = child->next_sibling;
    }
}

/**
 * @brief ���� Ʈ�� ���� ����� �����ϴ� ���� �Լ�
 * @param root Ʈ���� ��Ʈ ���
 */
void print_dynasty_tree(KingNode* root) {
    if (root == NULL) {
        printf("Ʈ���� ����ֽ��ϴ�.\n");
        return;
    }
    printf("\n--- ���� ���� Ʈ�� ���� ---\n");
    // ��Ʈ ��带 ���� ����մϴ�.
    printf("%s\n", root->name);

    // ��Ʈ�� ���� �ڽĵ���� ����� ����� �����մϴ�.
    KingNode* child = root->first_child;
    while (child != NULL) {
        int is_last = (child->next_sibling == NULL);
        print_tree_recursive(child, "", is_last);
        child = child->next_sibling;
    }
    printf("---------------------------\n\n");
}