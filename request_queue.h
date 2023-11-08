#pragma once

#include "search_server.h"
#include "document.h"

#include <deque>
#include <string>
#include <vector>

class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server);
    // сделаем "обёртки" для всех методов поиска, чтобы сохранять результаты для нашей статистики
    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
        std::vector<Document> result_list = search_server_.FindTopDocuments(raw_query, document_predicate);

        QueryResult result;

        if (result_list.empty()) {
            result.is_empty = true;
            ++empty_results_;
        }

        if (requests_.empty()) {
            requests_.push_back(result);
        }
        else {
            auto previous_request = requests_.back();
            result.time = previous_request.time + 1;
            result.query_count = previous_request.query_count + 1;
            

            if (min_in_day_ - result.time >= 0) {
                requests_.push_back(result);
            }
            else {
                if (requests_.front().is_empty) {
                    --empty_results_;
                }
                //result.time = 0;
                requests_.pop_front();
                requests_.push_back(result);
            }
        }
        return result_list;
    }
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status);
    std::vector<Document> AddFindRequest(const std::string& raw_query);
    int GetNoResultRequests() const;
private:
    struct QueryResult {
        int time = 1;
        int query_count = 1;
        bool is_empty = false;
    };
    
    std::deque<QueryResult> requests_;
    const static int min_in_day_ = 1440;
    const SearchServer& search_server_;
    unsigned int empty_results_ = 0;
};