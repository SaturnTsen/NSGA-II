#include "utils.h"
#include "benchmark.h"
#include "individual.h"
#include <cstddef>
#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>
using json = nlohmann::json;

std::string get_current_time() {
    std::time_t now = std::time(nullptr);
    std::tm *ltm = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(ltm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

namespace end_criteria {

    max_iterations::max_iterations(size_t max_iterations) : max_iters(max_iterations) {}

    bool max_iterations::operator()(const population_t &population, const size_t iteration) {
        return iteration >= max_iters;
    }

    cover_mlotz_pareto_front::cover_mlotz_pareto_front(size_t m) : m(m) {}

    size_t count_pareto_front(const population_t &p, size_t m) {
        size_t count_pareto_front = 0;
        for (const auto &individual : p)
            if (benchmark::is_mlotz_pareto_front(m, individual))
                count_pareto_front++;
        return count_pareto_front;
    }

    bool cover_mlotz_pareto_front::operator()(const population_t &p, const size_t iter) {
        using individual::operator<<;
        size_t cnt = count_pareto_front(p, m);
        if (iter % 20 == 0)
            std::cout << std::format("Iteration: {}, individuals on Pareto front: {}", iter, cnt)
                      << std::endl;
        if (cnt == p.size()) {
            for (auto &i : p) {
                std::cout << "individual: " << i << std::endl;
            }
            return true;
        }
        return false;
    }

    // Task6Logger(size_t id, size_t p, size_t m, size_t max_iters,
    // size_t print_period, std::string filename);
    Task6Logger::Task6Logger(const size_t id, const size_t p, const size_t m,
                             const size_t max_iters, const std::string filename,
                             const size_t print_period)
        : id(id), p(p), m(m), max_iters(max_iters), print_period(print_period), filename(filename) {
        log_data["metadata"]["begin_time"] = get_current_time();
        log_data["metadata"]["individual_size"] = id;
        log_data["metadata"]["population_size"] = p;
        log_data["metadata"]["objective_size"] = m;
        log_data["metadata"]["max_iters"] = max_iters;
        log_data["count_pareto_front"] = json::array();
        log_data["final_population"] = json::array();
    }

    void Task6Logger::sync_to_file() {
        std::ofstream log_file(filename, std::ios::trunc);
        if (log_file.is_open()) {
            log_file << log_data.dump(4) << std::endl;
            log_file.close();
        } else {
            std::cerr << "Unable to open log file: " << filename << std::endl;
        }
    }

    void Task6Logger::log_new_data(size_t optimum_count, const size_t current_iter) {
        log_data["count_pareto_front"].push_back(optimum_count);
        if (current_iter % print_period == 0) {
            std::cout << std::format("Iteration: {}, individuals on Pareto front: {}", current_iter,
                                     optimum_count)
                      << std::endl;
            sync_to_file();
        }
    }

    void Task6Logger::add_final_results(const population_t &population) {
        using individual::operator<<;
        for (const individual_t &individual : population) {
            auto result = individual::to_string(individual);
            if (!result.empty() && result[result.size() - 1] == '\n')
                result.erase(result.size() - 1);
            log_data["final_population"].push_back(result);
        }
        log_data["metadata"]["end_time"] = get_current_time();
        std::cout << "Saving log to " << filename << std::endl;
    }

    bool Task6Logger::operator()(const population_t &population, const size_t current_iter) {

        // Count the number of individuals in the Pareto set
        size_t cnt = count_pareto_front(population, m);
        log_new_data(cnt, current_iter);
        if (current_iter >= max_iters || cnt >= population.size()) {
            if (current_iter >= max_iters)
                std::cout << "Maximum iterations reached..." << std::endl;
            else
                std::cout << "All individuals are on the Pareto front!" << std::endl;

            // save final results
            add_final_results(population);
            sync_to_file();
            return true;
        }
        return false;
    }
} // namespace end_criteria