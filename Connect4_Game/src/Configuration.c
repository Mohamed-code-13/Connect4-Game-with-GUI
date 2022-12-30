#define _CRT_SECURE_NO_WARNINGS

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "include/Configuration.h"

Configuration config = { 4, 4, 10 };

int userTries = 0;

void getPathFromUser(char path[])
{
	printf("Enter the path for the xml File: ");
	fgets(path, 150, stdin);
	path[strlen(path) - 1] = 0;

	printf("The path is: %s\n", path);
}

void handleUserInput()
{
	printf("Invalid!\n");

	if (++userTries == 3)
	{
		printf("Loading default values. Height = 7, Width = 9, HighScores = 10\n");

		config.height = 7;
		config.width = 9;
		config.highScore = 10;

		return;
	}

	char path[151];
	getPathFromUser(path);
	readXML(path);
}

int hasChildren(xmlNode* node)
{
	for (xmlNode* cur_node = node; cur_node; cur_node =
		cur_node->next)
		if (cur_node->type == XML_ELEMENT_NODE)
			return 1;

	return 0;
}

int validDim(char* node)
{
	char* digits = strtok(node, " \n");

	for (int i = 0; i < strlen(digits); ++i)
		if (!isdigit(digits[i]))
			return -1;

	return atoi(digits);
}

int parseXML(xmlNode* node)
{
	for (xmlNode* cur_node = node; cur_node; cur_node =
		cur_node->next)
	{
		if (hasChildren(cur_node->children))
			return -1;

		if (cur_node->type == XML_ELEMENT_NODE)
		{
			int num = validDim(xmlNodeGetContent(cur_node));
			if (num == -1)
				return -1;
			
			if (!strcmp(cur_node->name, "Height"))
			{
				printf("Height: %d\n", num);
				config.height = num;
			}
			else if (!strcmp(cur_node->name, "Width"))
			{
				printf("Width: %d\n", num);
				config.width = num;
			}
			else if (!strcmp(cur_node->name, "Highscores"))
			{
				printf("Highscores: %d\n", num);
				config.highScore = num;
			}
		}
	}
	return 0;
}

// From stackoverflow: https://stackoverflow.com/questions/54380904/count-words-from-a-string-with-multiple-empty-spaces
int word_count(const char* s) 
{
	int count = 0;
	while (*(s += strspn(s, " \n")))  // Advance s by the matching delimiters.
	{
		count++;
		s += strcspn(s, " \n");  // Advance s by the non-matching delimiters.
	}
	return count;
}

void readXML(const char* xmlPath)
{
	xmlDoc* doc = NULL;
	xmlNode* root_element = NULL;

	doc = xmlReadFile(xmlPath, NULL, 0);
	
	if (doc == NULL) 
	{
		handleUserInput();

		return;
	}

	//Get the root element node
	root_element = xmlDocGetRootElement(doc);

	if (!strcmp(root_element->name, "Configurations"))
	{
		int contents_in_config = word_count(xmlNodeGetContent(root_element));
		if (contents_in_config != 3)
			handleUserInput();

		else
		{
			int state = parseXML(root_element->children);
			if (state == -1)
				handleUserInput();
		}
	}
	else
		handleUserInput();

	xmlFreeDoc(doc);    // free document
	xmlCleanupParser(); // Free globals
}