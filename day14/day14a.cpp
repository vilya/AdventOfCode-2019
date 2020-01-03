#include <algorithm>
#include <cstdio>
#include <string>
#include <map>
#include <vector>


struct Item {
  std::string name;
  uint64_t quantity;

  bool read(FILE* f) {
    char tmpName[16];
    int n = fscanf(f, "%llu %16s, ", &quantity, tmpName);
    if (n != 2) {
      return false;
    }
    name = tmpName;
    if (name.back() == ',') {
      name.pop_back();
    }
    return true;
  }
};


struct Recipe {
  std::vector<Item> ingredients; // Ingredients for this recipe
  Item result; // What this recipe produces
  int depth = -1;
};


static inline bool order_recipes_by_depth(const Recipe& a, const Recipe& b)
{
  return a.depth < b.depth;
}


struct Data {
  std::map<std::string, size_t> recipesByName;
  std::vector<Recipe> recipes;
  std::vector<uint64_t> used;

  bool load(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
      fprintf(stderr, "Couldn't open %s\n", filename);
      return false;
    }

    Recipe recipe;
    recipe.ingredients.reserve(16);
    while (!feof(f)) {
      recipe.ingredients.clear();
      if (recipe.result.read(f)) {
        do {
          recipe.ingredients.push_back(recipe.result);
        } while (recipe.result.read(f));
        fscanf(f, "=> ");
        if (!recipe.result.read(f)) {
          fclose(f);
          return false;
        }
        recipesByName[recipe.result.name] = recipes.size();
        recipes.push_back(recipe);
      }
    }
    fclose(f);

    // Add an empty recipe for ore.
    Recipe ore;
    ore.result.name = "ORE";
    ore.result.quantity = 1;
    ore.ingredients.clear();
    ore.depth = std::numeric_limits<int>::max();
    recipesByName[ore.result.name] = recipes.size();
    recipes.push_back(ore);

    // Topological sort of the list of recipes, so that if we're iterating over
    // the list of recipes we will always have processed all orders that have
    // recipe i as an ingredient before we process recipe i itself.
    set_max_depths("FUEL", 0);
    std::stable_sort(recipes.begin(), recipes.end(), order_recipes_by_depth);
    recipesByName.clear();
    for (size_t i = 0, endI = recipes.size(); i < endI; i++) {
      recipesByName[recipes[i].result.name] = i;
    }
    return true;
  }


  uint64_t cost_of_fuel(uint64_t n) {
    used.clear();
    used.resize(recipes.size(), 0ull);

    used[0] = n;
    for (size_t i = 0, endI = recipes.size(); i < endI; i++) {
      if (used[i] == 0) {
        continue;
      }
      uint64_t multiple = (used[i] + recipes[i].result.quantity - 1) / recipes[i].result.quantity;
      for (const Item& ingredient : recipes[i].ingredients) {
        size_t j = recipesByName[ingredient.name];
        used[j] += ingredient.quantity * multiple;
      }
    }
    return used.back(); // the last element of `used`, `excess` and `recipes` is always ore.
  }

private:
  void set_max_depths(const std::string& name, int depth) {
    Recipe& recipe = recipes[recipesByName.at(name)];
    if (depth > recipe.depth) {
      recipe.depth = depth;
    }
    ++depth;
    for (const Item& item : recipe.ingredients) {
      if (item.name == "ORE") {
        continue;
      }
      set_max_depths(item.name, depth);
    }
  }
};


int main(int argc, char** argv)
{
  Data data;
  if (!data.load(argv[1])) {
    fprintf(stderr, "Failed to load %s\n", argv[1]);
    return 1;
  }
  printf("1 fuel costs %llu ore\n", data.cost_of_fuel(1));
  return 0;
}
