#pragma once

#include <string>

namespace static_files {
	struct FileInfo {
		const char * fileName;
		const char * fileData;
		const unsigned int fileDataSize;

		std::string name() const {
			return fileName;
		}

		const std::string & content() const {
			static const std::string data{ fileData, fileDataSize };
			return data;
		}
	};

	extern const unsigned int fileInfoListSize;
	extern const FileInfo fileInfoList[];

	struct FileInfoRange {
		const FileInfo * begin() const {
			return &fileInfoList[0];
		}
		const FileInfo * end() const {
			return begin() + size();
		}
		const size_t size() const {
			return fileInfoListSize;
		}
	};

	inline FileInfoRange fileList() {
		return FileInfoRange{};
	}
}
