#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_RECIPE_NAME 100
#define MAX_INGREDIENTS 20
#define MAX_INSTRUCTIONS 50
#define MAX_RECIPES 100

struct Recipe {
    char name[MAX_RECIPE_NAME];
    char ingredients[MAX_INGREDIENTS][MAX_RECIPE_NAME];
    int num_ingredients;
    char instructions[MAX_INSTRUCTIONS][MAX_RECIPE_NAME];
    int num_steps;
    char category[MAX_RECIPE_NAME];
    int rating;
};

void displayHeader() {
    printf("\033[1;31m"); // Start ANSI escape code for red color
    printf("__          _             _   _ _ _ _       \n");
    printf("| _ \\        ()           | | | () | |      \n");
    printf("| |/ /_  __ _ _ _   __ | | | || | | _ _ \n");
    printf("|    // _ \\/ _| | ' \\ / _ \\| | | | | | |/ _` |\n");
    printf("| |\\ \\  _/ (| | |) |  _/\\ \\/ / | | | (_| |\n");
    printf("\\| \\\\_|\\_|| ./ \\_| \\_/||||\\_,|\n");
    printf("                 | |                            \n");
    printf("                 |_|                            \n");
    printf("\033[0m");
}

void print_recipe(struct Recipe r) {
    printf("  Name: %s\n", r.name);
    printf("  Ingredients:\n");
  for (int i = 0; i < r.num_ingredients; i++) {
    printf("    - %s\n", r.ingredients[i]);
  }
    printf("  Instructions:\n");
  for (int i = 0; i < r.num_steps; i++) {
    printf("    %d. %s\n", i + 1, r.instructions[i]);
  }
    printf("  Category: %s\n", r.category);
    printf("  Rating: %d/5\n", r.rating);
}



void search_by_ingredient(char ingredient[]) {
    FILE *file = fopen("recipes.bin", "rb");

    if (file == NULL) {
        printf("Unable to open file.\n");
        return;
    }

    struct Recipe tempRecipe;
    int found = 0;

    printf("Recipes containing ingredient '%s':\n", ingredient);
    while (fread(&tempRecipe, sizeof(struct Recipe), 1, file) == 1) {
        for (int i = 0; i < tempRecipe.num_ingredients; i++) {
            if (strcmp(tempRecipe.ingredients[i], ingredient) == 0) {
                print_recipe(tempRecipe);
                found = 1;
                break;
            }
        }
    }

    if (found == 0) {
        printf("No recipes found with ingredient '%s'\n", ingredient);
    }

    fclose(file);
}

void display_menu() {
    printf("\nWelcome to the Digital Recipe Book!\n");
    printf("Please choose an option:\n");
    printf("1. Add a new recipe\n");
    printf("2. View all recipes\n");
    printf("3. Search for a recipe by name\n");
    printf("4. Search for recipes by ingredient\n");
    printf("5. Edit a recipe\n");
    printf("6. Delete a recipe\n");
    printf("7. Exit\n");
}

void add_recipe(struct Recipe arr[], int *n) {
    if (*n >= MAX_RECIPES) {
        printf("Cannot add more recipes. Maximum limit reached.\n");
        return;
    }

    FILE *file = fopen("recipes.bin", "ab");

    if (file == NULL) {
        printf("Unable to open file.\n");
        return;
    }

    struct Recipe newRecipe;

    printf("Enter the name of the recipe: ");
    scanf("%s", newRecipe.name);

    printf("Enter the number of ingredients: ");
    scanf("%d", &newRecipe.num_ingredients);

    printf("Enter ingredients (one per line):\n");
    for (int i = 0; i < newRecipe.num_ingredients; i++) {
        scanf("%s", newRecipe.ingredients[i]);
    }

    printf("Enter the number of steps: ");
    scanf("%d", &newRecipe.num_steps);

    printf("Enter instructions (one per line):\n");
    for (int i = 0; i < newRecipe.num_steps; i++) {
        scanf("%s", newRecipe.instructions[i]);
    }

    printf("Enter the category: ");
    scanf("%s", newRecipe.category);

    printf("Enter the rating (out of 5): ");
    scanf("%d", &newRecipe.rating);

    fwrite(&newRecipe, sizeof(struct Recipe), 1, file);

    fclose(file);

    (*n)++;
    printf("Recipe added successfully!\n");
}


void view_recipes() {
    FILE *file = fopen("recipes.bin", "rb");

    if (file == NULL) {
        printf("Unable to open file.\n");
        return;
    }

    struct Recipe tempRecipe;

    printf("All Recipes:\n");
    int recipeCount = 0;
    while (fread(&tempRecipe, sizeof(struct Recipe), 1, file) == 1) {
        if (tempRecipe.name[0] != '\0') {
            printf("\nRecipe %d:\n", recipeCount + 1);
            print_recipe(tempRecipe);
            recipeCount++;
        }
    }

    if (recipeCount == 0) {
        printf("No recipes available!\n");
    }

    fclose(file);
}

void search_by_name(char search_name[]) {
    FILE *file = fopen("recipes.bin", "rb");

    if (file == NULL) {
        printf("Unable to open file.\n");
        return;
    }

    struct Recipe tempRecipe;
    int found = 0;

    while (fread(&tempRecipe, sizeof(struct Recipe), 1, file) == 1) {
        if (strcmp(tempRecipe.name, search_name) == 0) {
            printf("Recipe found with name '%s':\n", search_name);
            print_recipe(tempRecipe);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Recipe not found with name '%s'\n", search_name);
    }

    fclose(file);
}

void delete_recipe(const char *name) {
    FILE *file = fopen("recipes.bin", "rb+");

    if (file == NULL) {
        printf("Unable to open file.\n");
        return;
    }

    struct Recipe tempRecipe;
    bool found = false;
    int count = 0;

    while (fread(&tempRecipe, sizeof(struct Recipe), 1, file) == 1) {
        if (strcmp(tempRecipe.name, name) == 0) {
            found = true;
            fseek(file, count * sizeof(struct Recipe), SEEK_SET);
            struct Recipe emptyRecipe = {0};
            fwrite(&emptyRecipe, sizeof(struct Recipe), 1, file);
            printf("Recipe '%s' deleted successfully.\n", name);
            break;
        }
        count++;
    }

    if (!found) {
        printf("Recipe '%s' not found. Deletion failed.\n", name);
    }

    fclose(file);
}

void edit_recipe(int index) {
    FILE *file = fopen("recipes.bin", "rb+");

    if (file == NULL) {
        printf("Unable to open file.\n");
        return;
    }

    struct Recipe tempRecipe;
    int found = 0;

    while (fread(&tempRecipe, sizeof(struct Recipe), 1, file) == 1) {
        if (found == index) {
            printf("Enter the updated category: ");
            scanf("%s", tempRecipe.category);

            printf("Enter the updated rating (out of 5): ");
            scanf("%d", &tempRecipe.rating);

            fseek(file, -sizeof(struct Recipe), SEEK_CUR);
            fwrite(&tempRecipe, sizeof(struct Recipe), 1, file);
            printf("Recipe at number %d edited successfully.\n", index);
            found = 1;
            break;
        }
        found++;
    }

    if (!found) {
        printf("Recipe index %d not found. Edit failed.\n", index);
    }

    fclose(file);
}

int main() {
    struct Recipe recipes[MAX_RECIPES];
    int num_recipes = 0;
    int choice;

    do {
        system("cls");
        displayHeader();
        printf("Please choose an option:\n");

        display_menu();

        printf("Enter your choice: ");
        scanf("%d", &choice);

    switch (choice) {

        case 1:
            add_recipe(recipes, &num_recipes);
        break;

        case 2:
            view_recipes(recipes, num_recipes);
        break;

        case 3: {
            char search_name[MAX_RECIPE_NAME];
            printf("Enter the name of the recipe to search: ");
            scanf("%s", search_name);

            search_by_name(search_name);
            break;
        }

        case 4: {
            char search_ingredient[MAX_RECIPE_NAME];
            printf("Enter the ingredient to search for: ");
            scanf("%s", search_ingredient);

            search_by_ingredient(search_ingredient);
            break;
        }

        case 5: {
            int edit_index;
            printf("Enter the number of the recipe to edit: ");
            scanf("%d", &edit_index);
            edit_index--;
            edit_recipe(edit_index);
            break;
        }

        case 6: {
            char delete_name[MAX_RECIPE_NAME];
            printf("Enter the name of the recipe to delete: ");
            scanf("%s", delete_name);

            delete_recipe(delete_name);
            break;
        }

        case 7:
            printf("Thank you for using the RecipeVilla!\n");
            return 0;
            break;

        default:
            printf("Invalid choice. Please enter a valid option.\n");
            break;
    }
        printf("\nPress Enter to continue...");

        while (getchar() != '\n');
        getchar();

        system("cls");

    } while (1);
  return 0;
}
