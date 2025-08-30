#include <stdio.h>
#include <string.h>

#define MAX_SCRIPTS 30
#define MAX_VERSIONS 20
#define MAX_CONFIG_KV 50

/* ------------ Data Models ------------ */

typedef struct {
    char key[32];
    char value[128];
} KVPair;

typedef struct {
    int version;
    char message[120];
    char content[512];       // test script content (or path)
} ScriptVersion;

typedef struct {
    char name[64];           // unique script name
    int versionCount;
    ScriptVersion versions[MAX_VERSIONS];
} Script;

typedef struct {
    int version;
    char message[120];
    KVPair kv[MAX_CONFIG_KV];
    int kvCount;
} ConfigSnapshot;

typedef struct {
    Script scripts[MAX_SCRIPTS];
    int scriptCount;

    ConfigSnapshot configs[MAX_VERSIONS];
    int configCount;
} Repo;

/* ------------ Helpers ------------ */

void trim_newline(char *s){
    size_t n = strlen(s);
    if(n && s[n-1]=='\n') s[n-1]=0;
}

int find_script(Repo *repo, const char *name){
    int i=0;
    while(i<repo->scriptCount){
        if(strcmp(repo->scripts[i].name, name)==0) return i;
        i++;
    }
    return -1;
}

void print_script(Script *s){
    if(s->versionCount==0){
        printf("  (no versions)\n");
        return;
    }
    ScriptVersion *v = &s->versions[s->versionCount-1];
    printf("  %s @ v%d | \"%s\"\n", s->name, v->version, v->message);
}

void list_scripts(Repo *repo){
    if(repo->scriptCount==0){
        printf("No scripts yet.\n");
        return;
    }
    int i=0;
    while(i<repo->scriptCount){
        print_script(&repo->scripts[i]);
        i++;
    }
}

void show_script_history(Repo *repo){
    char name[64];
    printf("Enter script name: ");
    fgets(name, sizeof(name), stdin); trim_newline(name);

    int idx = find_script(repo, name);
    if(idx<0){
        printf("Script not found.\n");
        return;
    }
    Script *s = &repo->scripts[idx];
    if(s->versionCount==0){
        printf("No versions.\n");
        return;
    }
    int i=0;
    while(i<s->versionCount){
        printf("v%-3d  msg: %s\n", s->versions[i].version, s->versions[i].message);
        i++;
    }
}

void add_script(Repo *repo){
    if(repo->scriptCount>=MAX_SCRIPTS){
        printf("Script limit reached.\n");
        return;
    }
    char name[64], content[512], msg[120];
    printf("New script name: ");
    fgets(name, sizeof(name), stdin); trim_newline(name);

    if(find_script(repo, name)>=0){
        printf("Script already exists.\n");
        return;
    }

    printf("Initial content/path: ");
    fgets(content, sizeof(content), stdin); trim_newline(content);

    printf("Commit message: ");
    fgets(msg, sizeof(msg), stdin); trim_newline(msg);

    Script *s = &repo->scripts[repo->scriptCount];
    strcpy(s->name, name);
    s->versionCount = 1;
    s->versions[0].version = 1;
    strcpy(s->versions[0].content, content);
    strcpy(s->versions[0].message, msg);

    repo->scriptCount++;
    printf("Script '%s' added at v1.\n", name);
}

void update_script(Repo *repo){
    char name[64], content[512], msg[120];
    printf("Script name to update: ");
    fgets(name, sizeof(name), stdin); trim_newline(name);

    int idx = find_script(repo, name);
    if(idx<0){
        printf("Script not found.\n");
        return;
    }
    Script *s = &repo->scripts[idx];
    if(s->versionCount>=MAX_VERSIONS){
        printf("Version limit reached.\n");
        return;
    }

    printf("New content/path: ");
    fgets(content, sizeof(content), stdin); trim_newline(content);

    printf("Commit message: ");
    fgets(msg, sizeof(msg), stdin); trim_newline(msg);

    int v = s->versionCount;
    s->versions[v].version = v+1;
    strcpy(s->versions[v].content, content);
    strcpy(s->versions[v].message, msg);
    s->versionCount++;

    printf("Updated '%s' to v%d.\n", s->name, v+1);
}

void checkout_script(Repo *repo){
    char name[64];
    int target;
    printf("Script name to checkout: ");
    fgets(name, sizeof(name), stdin); trim_newline(name);

    int idx = find_script(repo, name);
    if(idx<0){
        printf("Script not found.\n");
        return;
    }
    Script *s = &repo->scripts[idx];
    if(s->versionCount==0){
        printf("No versions.\n");
        return;
    }
    printf("Target version (1..%d): ", s->versionCount);
    if(scanf("%d", &target)!=1){ while(getchar()!='\n'); printf("Invalid input.\n"); return; }
    while(getchar()!='\n');

    if(target<1 || target> s->versionCount){
        printf("Invalid version.\n");
        return;
    }

    // Truncate history to selected version (like hard reset)
    s->versionCount = target;
    printf("Checked out '%s' to v%d.\n", s->name, target);
}

void show_script_current(Repo *repo){
    char name[64];
    printf("Script name: ");
    fgets(name, sizeof(name), stdin); trim_newline(name);

    int idx = find_script(repo, name);
    if(idx<0){
        printf("Script not found.\n");
        return;
    }
    Script *s = &repo->scripts[idx];
    if(s->versionCount==0){
        printf("No versions.\n");
        return;
    }
    ScriptVersion *v = &s->versions[s->versionCount-1];
    printf("Name: %s\nVersion: %d\nMessage: %s\nContent:\n%s\n",
           s->name, v->version, v->message, v->content);
}

/* ------------ Config Management ------------ */

void config_set_key(ConfigSnapshot *snap){
    if(snap->kvCount>=MAX_CONFIG_KV){
        printf("KV capacity reached.\n");
        return;
    }
    char key[32], val[128];
    printf("Key: ");
    fgets(key, sizeof(key), stdin); trim_newline(key);
    printf("Value: ");
    fgets(val, sizeof(val), stdin); trim_newline(val);

    // Upsert
    int i=0, found=-1;
    while(i<snap->kvCount){
        if(strcmp(snap->kv[i].key, key)==0) found=i;
        i++;
    }
    if(found>=0){
        strcpy(snap->kv[found].value, val);
        printf("Updated %s.\n", key);
    } else {
        strcpy(snap->kv[snap->kvCount].key, key);
        strcpy(snap->kv[snap->kvCount].value, val);
        snap->kvCount++;
        printf("Added %s.\n", key);
    }
}

void show_working_config(ConfigSnapshot *snap){
    if(snap->kvCount==0){
        printf("(empty)\n");
        return;
    }
    int i=0;
    while(i<snap->kvCount){
        printf("%s = %s\n", snap->kv[i].key, snap->kv[i].value);
        i++;
    }
}

void commit_config(Repo *repo, ConfigSnapshot *working){
    if(repo->configCount>=MAX_VERSIONS){
        printf("Config version limit reached.\n");
        return;
    }
    char msg[120];
    printf("Commit message: ");
    fgets(msg, sizeof(msg), stdin); trim_newline(msg);

    ConfigSnapshot *dst = &repo->configs[repo->configCount];
    dst->version = repo->configCount + 1;
    strcpy(dst->message, msg);
    dst->kvCount = working->kvCount;

    int i=0;
    while(i<working->kvCount){
        dst->kv[i] = working->kv[i];
        i++;
    }

    repo->configCount++;
    printf("Committed config snapshot v%d.\n", dst->version);
}

void show_config_history(Repo *repo){
    if(repo->configCount==0){
        printf("No config commits.\n");
        return;
    }
    int i=0;
    while(i<repo->configCount){
        printf("v%-3d  msg: %s  (entries: %d)\n",
               repo->configs[i].version, repo->configs[i].message, repo->configs[i].kvCount);
        i++;
    }
}

void checkout_config(Repo *repo, ConfigSnapshot *working){
    if(repo->configCount==0){
        printf("No config commits to checkout.\n");
        return;
    }
    int t;
    printf("Target config version (1..%d): ", repo->configCount);
    if(scanf("%d", &t)!=1){ while(getchar()!='\n'); printf("Invalid input.\n"); return; }
    while(getchar()!='\n');

    if(t<1 || t>repo->configCount){
        printf("Invalid version.\n");
        return;
    }
    ConfigSnapshot *src = &repo->configs[t-1];
    working->kvCount = src->kvCount;
    int i=0;
    while(i<src->kvCount){
        working->kv[i] = src->kv[i];
        i++;
    }
    printf("Checked out config to v%d (not a new commit; working copy updated).\n", t);
}

/* ------------ Main ------------ */

int main(){
    Repo repo;
    repo.scriptCount = 0;
    repo.configCount = 0;

    // Working (uncommitted) environment config
    ConfigSnapshot working;
    working.version = 0;
    strcpy(working.message, "working");
    working.kvCount = 0;

    int choice = 0;
    int running = 1;

    while(running){
        printf("\n=== Test Assets VC & Config Manager ===\n");
        printf("1  Add script\n");
        printf("2  Update script (new version)\n");
        printf("3  Show script history\n");
        printf("4  Checkout script version (hard reset)\n");
        printf("5  Show current script content\n");
        printf("6  Config: set/update key\n");
        printf("7  Config: show working copy\n");
        printf("8  Config: commit snapshot\n");
        printf("9  Config: history\n");
        printf("10 Config: checkout snapshot to working\n");
        printf("11 List scripts (summary)\n");
        printf("12 Exit\n");
        printf("Enter choice: ");

        if(scanf("%d", &choice)!=1){
            while(getchar()!='\n');
            printf("Invalid input.\n");
        } else {
            while(getchar()!='\n');  // clear line

            if(choice==1) add_script(&repo);
            else if(choice==2) update_script(&repo);
            else if(choice==3) show_script_history(&repo);
            else if(choice==4) checkout_script(&repo);
            else if(choice==5) show_script_current(&repo);
            else if(choice==6) config_set_key(&working);
            else if(choice==7) show_working_config(&working);
            else if(choice==8) commit_config(&repo, &working);
            else if(choice==9) show_config_history(&repo);
            else if(choice==10) checkout_config(&repo, &working);
            else if(choice==11) list_scripts(&repo);
            else if(choice==12) running = 0;
            else printf("Invalid choice.\n");
        }
    }

    printf("Goodbye.\n");
    return 0;
}
