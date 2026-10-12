#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* =========================================
   GET VALUE FROM JSON
   ========================================= */

void getValue(char *data, char *key, char *result)
{
    char search[100];

    sprintf(search, "\"%s\":", key);

    char *pos = strstr(data, search);

    if (pos == NULL)
    {
        strcpy(result, "Not Available");
        return;
    }

    pos += strlen(search);

    while (*pos == ' ' || *pos == '\t')
        pos++;

    if (strncmp(pos, "null", 4) == 0)
    {
        strcpy(result, "Not Available");
        return;
    }

    if (*pos == '"')
    {
        pos++;

        int i = 0;

        while (*pos != '"' &&
               *pos != '\0' &&
               i < 199)
        {
            result[i++] = *pos++;
        }

        result[i] = '\0';
    }
    else
    {
        int i = 0;

        while (*pos != ',' &&
               *pos != '}' &&
               *pos != '\0' &&
               i < 199)
        {
            result[i++] = *pos++;
        }

        result[i] = '\0';
    }
}


/* =========================================
   SUM JSON NUMBERS
   ========================================= */

long long sumJsonValues(char *data, char *key)
{
    char search[100];

    sprintf(search, "\"%s\":", key);

    char *pos = data;

    long long total = 0;

    while ((pos = strstr(pos, search)) != NULL)
    {
        pos += strlen(search);

        while (*pos == ' ' || *pos == '\t')
            pos++;

        if (*pos >= '0' && *pos <= '9')
        {
            long long value = 0;

            while (*pos >= '0' && *pos <= '9')
            {
                value = value * 10 + (*pos - '0');
                pos++;
            }

            total += value;
        }
    }

    return total;
}


/* =========================================
   COUNT PROGRAMMING LANGUAGES
   ========================================= */

void analyzeLanguages(
    char *data,
    int *c,
    int *cpp,
    int *java,
    int *python,
    int *javascript,
    int *html,
    int *css,
    int *other)
{
    *c = 0;
    *cpp = 0;
    *java = 0;
    *python = 0;
    *javascript = 0;
    *html = 0;
    *css = 0;
    *other = 0;

    char *pos = data;

    while ((pos = strstr(pos, "\"language\":")) != NULL)
    {
        pos += strlen("\"language\":");

        while (*pos == ' ' || *pos == '\t')
            pos++;

        if (*pos == '"')
        {
            pos++;

            char language[100];

            int i = 0;

            while (*pos != '"' &&
                   *pos != '\0' &&
                   i < 99)
            {
                language[i++] = *pos++;
            }

            language[i] = '\0';

            if (strcmp(language, "C") == 0)
                (*c)++;

            else if (strcmp(language, "C++") == 0)
                (*cpp)++;

            else if (strcmp(language, "Java") == 0)
                (*java)++;

            else if (strcmp(language, "Python") == 0)
                (*python)++;

            else if (strcmp(language, "JavaScript") == 0)
                (*javascript)++;

            else if (strcmp(language, "HTML") == 0)
                (*html)++;

            else if (strcmp(language, "CSS") == 0)
                (*css)++;

            else
                (*other)++;
        }
    }
}


/* =========================================
   FIND MOST USED LANGUAGE
   ========================================= */

void findMostUsedLanguage(
    int c,
    int cpp,
    int java,
    int python,
    int javascript,
    int html,
    int css,
    int other,
    char *result)
{
    int max = c;

    strcpy(result, "C");

    if (cpp > max)
    {
        max = cpp;
        strcpy(result, "C++");
    }

    if (java > max)
    {
        max = java;
        strcpy(result, "Java");
    }

    if (python > max)
    {
        max = python;
        strcpy(result, "Python");
    }

    if (javascript > max)
    {
        max = javascript;
        strcpy(result, "JavaScript");
    }

    if (html > max)
    {
        max = html;
        strcpy(result, "HTML");
    }

    if (css > max)
    {
        max = css;
        strcpy(result, "CSS");
    }

    if (other > max)
    {
        max = other;
        strcpy(result, "Other");
    }

    if (max == 0)
    {
        strcpy(result, "Not Available");
    }
}


/* =========================================
   ACCOUNT AGE
   ========================================= */

int calculateAccountAge(char *createdDate)
{
    if (strcmp(createdDate, "Not Available") == 0)
        return 0;

    int year;
    int month;
    int day;

    if (sscanf(createdDate,
               "%d-%d-%d",
               &year,
               &month,
               &day) != 3)
    {
        return 0;
    }

    time_t currentTime = time(NULL);

    struct tm *currentDate =
        localtime(&currentTime);

    int currentYear =
        currentDate->tm_year + 1900;

    int currentMonth =
        currentDate->tm_mon + 1;

    int currentDay =
        currentDate->tm_mday;

    int age = currentYear - year;

    if (currentMonth < month ||
        (currentMonth == month &&
         currentDay < day))
    {
        age--;
    }

    return age;
}


/* =========================================
   ACTIVITY STATUS
   ========================================= */

void getActivityStatus(
    int repos,
    int followers,
    int following,
    char *status)
{
    if (repos >= 20 && followers >= 100)
    {
        strcpy(status, "Highly Active");
    }
    else if (repos >= 10 || followers >= 50)
    {
        strcpy(status, "Active");
    }
    else if (repos >= 5 || following >= 20)
    {
        strcpy(status, "Moderately Active");
    }
    else
    {
        strcpy(status, "Low Activity");
    }
}


/* =========================================
   MAIN
   ========================================= */

int main()
{
    char username[100];

    printf("============================================\n");
    printf("         GITHUB DEVELOPER ANALYZER\n");
    printf("============================================\n\n");

    printf("Enter GitHub Username: ");

    scanf("%99s", username);


    /* =====================================
       FETCH PROFILE DATA
       ===================================== */

    printf("\nFetching GitHub profile data...\n");

    char command[500];

    sprintf(command,
            "curl -s https://api.github.com/users/%s > github_data.json",
            username);

    system(command);


    FILE *file =
        fopen("github_data.json", "r");

    if (file == NULL)
    {
        printf("\nError: Could not read GitHub data.\n");
        return 1;
    }


    char data[30000];

    size_t size =
        fread(data,
              1,
              sizeof(data) - 1,
              file);

    data[size] = '\0';

    fclose(file);


    /* =====================================
       INVALID USERNAME CHECK
       ===================================== */

    if (strstr(data, "\"message\"") != NULL)
    {
        printf("\nGitHub user not found!\n");
        printf("Please check the username.\n");

        return 1;
    }


    /* =====================================
       PROFILE INFORMATION
       ===================================== */

    char name[200];
    char location[200];
    char bio[500];
    char repos[200];
    char followers[200];
    char following[200];
    char gists[200];
    char profileUrl[300];
    char createdDate[200];


    getValue(data, "name", name);
    getValue(data, "location", location);
    getValue(data, "bio", bio);
    getValue(data, "public_repos", repos);
    getValue(data, "followers", followers);
    getValue(data, "following", following);
    getValue(data, "public_gists", gists);
    getValue(data, "html_url", profileUrl);
    getValue(data, "created_at", createdDate);


    /* =====================================
       CONVERT VALUES
       ===================================== */

    int repoCount = atoi(repos);
    int followerCount = atoi(followers);
    int followingCount = atoi(following);


    /* =====================================
       ACCOUNT AGE
       ===================================== */

    int accountAge =
        calculateAccountAge(createdDate);


    /* =====================================
       ACTIVITY STATUS
       ===================================== */

    char activityStatus[50];

    getActivityStatus(
        repoCount,
        followerCount,
        followingCount,
        activityStatus);


    /* =====================================
       PROFILE DISPLAY
       ===================================== */

    printf("\n============================================\n");
    printf("          GITHUB DEVELOPER PROFILE\n");
    printf("============================================\n");

    printf("Username        : %s\n", username);
    printf("Name            : %s\n", name);
    printf("Location        : %s\n", location);
    printf("Bio             : %s\n", bio);
    printf("Public Repos    : %s\n", repos);
    printf("Followers       : %s\n", followers);
    printf("Following       : %s\n", following);
    printf("Public Gists    : %s\n", gists);
    printf("GitHub Profile  : %s\n", profileUrl);
    printf("Account Created : %s\n", createdDate);


    /* =====================================
       ACCOUNT & ACTIVITY ANALYSIS
       ===================================== */

    printf("\n============================================\n");
    printf("        ACCOUNT & ACTIVITY ANALYSIS\n");
    printf("============================================\n");

    if (accountAge > 0)
        printf("Account Age     : %d years\n", accountAge);
    else
        printf("Account Age     : Not Available\n");

    printf("Activity Status : %s\n",
           activityStatus);


    if (repoCount >= 20)
        printf("Repository Activity : High\n");
    else if (repoCount >= 10)
        printf("Repository Activity : Medium\n");
    else
        printf("Repository Activity : Low\n");


    /* =====================================
       DEVELOPER SCORE
       ===================================== */

    int score = 0;


    if (repoCount >= 50)
        score += 40;

    else if (repoCount >= 20)
        score += 30;

    else if (repoCount >= 10)
        score += 20;

    else
        score += 10;


    if (followerCount >= 1000)
        score += 40;

    else if (followerCount >= 500)
        score += 30;

    else if (followerCount >= 100)
        score += 20;

    else
        score += 10;


    if (followingCount >= 100)
        score += 20;

    else if (followingCount >= 50)
        score += 15;

    else
        score += 10;


    /* =====================================
       DEVELOPER LEVEL
       ===================================== */

    char level[30];


    if (score >= 70)
        strcpy(level, "Advanced");

    else if (score >= 40)
        strcpy(level, "Intermediate");

    else
        strcpy(level, "Beginner");


    /* =====================================
       FETCH REPOSITORIES
       ===================================== */

    printf("\nFetching repository data...\n");


    char repoCommand[500];


    sprintf(repoCommand,
            "curl -s \"https://api.github.com/users/%s/repos?per_page=100\" > repositories.json",
            username);

    system(repoCommand);


    FILE *repoFile =
        fopen("repositories.json", "r");


    long long totalStars = 0;
    long long totalForks = 0;


    int c = 0;
    int cpp = 0;
    int java = 0;
    int python = 0;
    int javascript = 0;
    int html = 0;
    int css = 0;
    int other = 0;


    char popularRepo[200];
    char popularStars[200];


    strcpy(popularRepo, "Not Available");
    strcpy(popularStars, "0");


    if (repoFile != NULL)
    {
        char repoData[100000];


        size_t repoSize =
            fread(repoData,
                  1,
                  sizeof(repoData) - 1,
                  repoFile);


        repoData[repoSize] = '\0';

        fclose(repoFile);


        /* Total Stars */

        totalStars =
            sumJsonValues(repoData,
                          "stargazers_count");


        /* Total Forks */

        totalForks =
            sumJsonValues(repoData,
                          "forks_count");


        /* Language Analysis */

        analyzeLanguages(
            repoData,
            &c,
            &cpp,
            &java,
            &python,
            &javascript,
            &html,
            &css,
            &other);


        /* =================================
           MOST POPULAR REPOSITORY
           ================================= */

        char popularCommand[600];


        sprintf(
            popularCommand,
            "curl -s \"https://api.github.com/search/repositories?q=user:%s&sort=stars&order=desc&per_page=1\" > popular_repo.json",
            username);


        system(popularCommand);


        FILE *popularFile =
            fopen("popular_repo.json", "r");


        if (popularFile != NULL)
        {
            char popularData[30000];


            size_t popularSize =
                fread(popularData,
                      1,
                      sizeof(popularData) - 1,
                      popularFile);


            popularData[popularSize] = '\0';

            fclose(popularFile);


            getValue(
                popularData,
                "name",
                popularRepo);


            getValue(
                popularData,
                "stargazers_count",
                popularStars);
        }
    }


    /* =====================================
       LANGUAGE DIVERSITY
       ===================================== */

    int languageDiversity = 0;

    if (c > 0)
        languageDiversity++;

    if (cpp > 0)
        languageDiversity++;

    if (java > 0)
        languageDiversity++;

    if (python > 0)
        languageDiversity++;

    if (javascript > 0)
        languageDiversity++;

    if (html > 0)
        languageDiversity++;

    if (css > 0)
        languageDiversity++;

    if (other > 0)
        languageDiversity++;


    /* =====================================
       PERFORMANCE CALCULATIONS
       ===================================== */

    double averageStars = 0;
    double averageForks = 0;
    double followersPerRepo = 0;


    if (repoCount > 0)
    {
        averageStars =
            (double)totalStars / repoCount;

        averageForks =
            (double)totalForks / repoCount;

        followersPerRepo =
            (double)followerCount / repoCount;
    }


    /* =====================================
       MOST USED LANGUAGE
       ===================================== */

    char mostUsedLanguage[100];


    findMostUsedLanguage(
        c,
        cpp,
        java,
        python,
        javascript,
        html,
        css,
        other,
        mostUsedLanguage);


    /* =====================================
       POPULARITY RATING
       ===================================== */

    char popularity[30];


    if (totalStars >= 1000 ||
        followerCount >= 1000)
    {
        strcpy(popularity, "Excellent");
    }

    else if (totalStars >= 100 ||
             followerCount >= 100)
    {
        strcpy(popularity, "Good");
    }

    else if (totalStars >= 20 ||
             followerCount >= 20)
    {
        strcpy(popularity, "Average");
    }

    else
    {
        strcpy(popularity, "Low");
    }


    /* =====================================
       DEVELOPER ANALYSIS
       ===================================== */

    printf("\n============================================\n");
    printf("             DEVELOPER ANALYSIS\n");
    printf("============================================\n");

    printf("Developer Score : %d / 100\n",
           score);

    printf("Developer Level : %s\n",
           level);


    printf("\nRepository Activity : ");


    if (repoCount >= 20)
        printf("Excellent\n");

    else if (repoCount >= 10)
        printf("Good\n");

    else if (repoCount >= 5)
        printf("Average\n");

    else
        printf("Low\n");


    /* =====================================
       REPOSITORY ANALYSIS
       ===================================== */

    printf("\n============================================\n");
    printf("            REPOSITORY ANALYSIS\n");
    printf("============================================\n");

    printf("Total Repositories : %d\n",
           repoCount);

    printf("Total Stars        : %lld\n",
           totalStars);

    printf("Total Forks        : %lld\n",
           totalForks);

    printf("Most Popular Repo  : %s\n",
           popularRepo);

    printf("Popular Repo Stars : %s\n",
           popularStars);


    /* =====================================
       PROGRAMMING LANGUAGE ANALYSIS
       ===================================== */

    printf("\n============================================\n");
    printf("       PROGRAMMING LANGUAGE ANALYSIS\n");
    printf("============================================\n");

    printf("C            : %d repositories\n", c);
    printf("C++          : %d repositories\n", cpp);
    printf("Java         : %d repositories\n", java);
    printf("Python       : %d repositories\n", python);
    printf("JavaScript   : %d repositories\n", javascript);
    printf("HTML         : %d repositories\n", html);
    printf("CSS          : %d repositories\n", css);
    printf("Other        : %d repositories\n", other);

    printf("\nMost Used Language : %s\n",
           mostUsedLanguage);


    /* =====================================
       DEVELOPER PERFORMANCE
       ===================================== */

    printf("\n============================================\n");
    printf("        DEVELOPER PERFORMANCE ANALYSIS\n");
    printf("============================================\n");

    printf("Average Stars/Repo  : %.2f\n",
           averageStars);

    printf("Average Forks/Repo  : %.2f\n",
           averageForks);

    printf("Followers/Repo      : %.2f\n",
           followersPerRepo);

    printf("Language Diversity  : %d languages\n",
           languageDiversity);

    printf("Popularity Rating   : %s\n",
           popularity);


    /* =====================================
       DEVELOPER INSIGHTS
       ===================================== */

    printf("\n============================================\n");
    printf("             DEVELOPER INSIGHTS\n");
    printf("============================================\n");

    if (repoCount >= 20)
        printf("- Strong repository portfolio.\n");
    else
        printf("- Portfolio can be expanded with more projects.\n");

    if (totalStars >= 100)
        printf("- Projects have received good community attention.\n");
    else
        printf("- Focus on useful projects to increase visibility.\n");

    if (languageDiversity >= 3)
        printf("- Good programming language diversity.\n");
    else
        printf("- Consider exploring additional technologies.\n");

    if (averageStars >= 50)
        printf("- Good average project popularity.\n");
    else
        printf("- Improve project documentation and presentation.\n");

    if (accountAge >= 5)
        printf("- Long-standing GitHub account.\n");
    else
        printf("- Relatively newer GitHub account.\n");


    /* =====================================
       RECOMMENDATIONS
       ===================================== */

    printf("\n============================================\n");
    printf("          DEVELOPER RECOMMENDATIONS\n");
    printf("============================================\n");

    if (repoCount < 10)
        printf("- Add more projects to your GitHub profile.\n");
    else
        printf("- Maintain your project portfolio.\n");

    if (followerCount < 100)
        printf("- Increase your GitHub network.\n");
    else
        printf("- Continue engaging with the GitHub community.\n");

    if (totalStars < 100)
        printf("- Create projects with clear README documentation.\n");
    else
        printf("- Continue improving popular projects.\n");

    if (languageDiversity <= 1)
        printf("- Explore more programming languages.\n");
    else
        printf("- Maintain your technology diversity.\n");

    printf("- Keep repositories updated regularly.\n");


    /* =====================================
       FINAL DEVELOPER REPORT
       ===================================== */

    printf("\n============================================\n");
    printf("           FINAL DEVELOPER REPORT\n");
    printf("============================================\n");

    printf("Developer Level : %s\n",
           level);

    printf("Developer Score : %d / 100\n",
           score);

    printf("Main Language   : %s\n",
           mostUsedLanguage);

    printf("Repositories    : %d\n", repoCount);

    printf("Total Stars     : %lld\n", totalStars);

    printf("Total Forks     : %lld\n", totalForks);

    printf("Account Age     : %d years\n", accountAge);

    printf("Activity Status : %s\n", activityStatus);

    printf("Popularity      : %s\n", popularity);

    printf("GitHub Profile  : %s\n", profileUrl);


    /* =====================================
       ANALYSIS COMPLETE
       ===================================== */

    printf("\n============================================\n");
    printf("            ANALYSIS COMPLETE\n");
    printf("============================================\n");

    return 0;
}