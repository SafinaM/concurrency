//
// Created by mar on 28.07.19.
//
#include <stdio.h>
#include <thread>
#include <iostream>

#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>


void threadTask(const char* path) {
	if (path == NULL)
		return;
	std::cout << "copying: " << path << std::endl;
	return;
}

void listdir(const char *name, int indent)
{
	DIR *dir;
	struct dirent *entry;
	
	if (!(dir = opendir(name)))
		return;
	
	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_type == DT_DIR) {
			char path[1024];
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
				continue;
			snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
			std::thread t(threadTask, entry->d_name);
			t.join();
//			printf("%*s[%s]\n", indent, "", entry->d_name);
			listdir(path, indent + 2);
		} else if (entry->d_type == DT_FIFO ) {
			std::cout << "pipes "<< entry->d_name << std::endl;
		} else if (entry->d_type == DT_LNK) {
			std::cout << "links: "<< entry->d_name << std::endl;
		} else {
//			printf("%*s- %s\n", indent, "", entry->d_name);
			std::thread t(threadTask, entry->d_name);
			t.join();
		}
	}
	closedir(dir);
}

int main(int argc, char** argv) {
	
	if (argc < 3)
		std::cout << "Point two directories." << std::endl;
	const std::string inputDir = argv[1];
	const std::string dstDir = argv[2];
	
	listdir("/home/mar/", 0);
	
	return 0;
}
