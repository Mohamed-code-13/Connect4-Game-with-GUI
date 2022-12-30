#define _CRT_SECURE_NO_WARNINGS

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <string.h>
#include "include/Configuration.h"

Configuration config = { 4, 4, 10 };

int hasChildren(xmlNode* node)
{
	for (xmlNode* cur_node = node; cur_node; cur_node =
		cur_node->next)
		if (cur_node->type == XML_ELEMENT_NODE)
			return 1;

	return 0;
}

void parseXML(xmlNode* node)
{

	for (xmlNode* cur_node = node; cur_node; cur_node =
		cur_node->next)
	{
		if (hasChildren(cur_node->children))
		{
			printf("Invalid!\n");
			return;
		}
		if (cur_node->type == XML_ELEMENT_NODE)
		{
			int num = atoi(strtok(xmlNodeGetContent(cur_node), " \n"));
			
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
}

// From stackoverflow: https://stackoverflow.com/questions/54380904/count-words-from-a-string-with-multiple-empty-spaces
int word_count(const char* s) {
	int count = 0;
	while (*(s += strspn(s, " \n"))) {  // Advance s by the matching delimiters.
		count++;
		s += strcspn(s, " \n");  // Advance s by the non-matching delimiters.
	}
	return count;
}

void readXML()
{
	xmlDoc* doc = NULL;
	xmlNode* root_element = NULL;

	doc = xmlReadFile("Data/Configurations/config.xml", NULL, 0);
	
	if (doc == NULL) {
		printf("Invalid\n");
		return;
	}

	//Get the root element node
	root_element = xmlDocGetRootElement(doc);

	if (!strcmp(root_element->name, "Configurations"))
	{
		int contents_in_config = word_count(xmlNodeGetContent(root_element));
		if (contents_in_config != 3)
			printf("Invalid!\n");
		else
			parseXML(root_element->children);
	}
	else
		printf("Invalid!\n");

	xmlFreeDoc(doc);    // free document
	xmlCleanupParser(); // Free globals
}