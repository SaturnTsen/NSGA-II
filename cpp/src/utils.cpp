#include "utils.h"
#include "benchmark.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>
using json = nlohmann::json;

namespace end_criteria {

    max_iterations::max_iterations(size_t max_iterations)
        : max_iters(max_iterations) {}

    bool max_iterations::operator()(population_t &population,
                                    const size_t iteration) {
        return iteration >= max_iters;
    }

    hit_mlotz_pareto_front::hit_mlotz_pareto_front(size_t m) : m(m) {}

    bool hit_mlotz_pareto_front::operator()(population_t &population,
                                            const size_t iteration) {

        using individual::operator<<;
        size_t count_pareto_front = 0;

        for (individual_t &individual : population) {
            if (benchmark::is_mlotz_pareto_front(m, individual))
                count_pareto_front++;
        }

        if (iteration % 20 == 0)
            std::cout << std::format(
                             "iteration: {}, individuals on Pareto front: {}",
                             iteration, count_pareto_front)
                      << std::endl;

        if (count_pareto_front == population.size()) {
            for (individual_t &individual : population) {
                std::cout << "individual: " << individual << std::endl;
            }
            return true;
        }

        return false;
    }

    // Task6Logger(size_t id, size_t p, size_t m, size_t max_iters,
    // size_t print_period, std::string filename);
    Task6Logger::Task6Logger(size_t id, size_t p, size_t m, size_t max_iters,
                             std::string filename, size_t print_period)
        : id(id), p(p), m(m), max_iters(max_iters), print_period(print_period),
          filename(filename) {
        std::time_t now = std::time(nullptr);
        std::tm *ltm = std::localtime(&now);
        std::ostringstream oss;
        oss << std::put_time(ltm, "%Y-%m-%d %H:%M:%S");
        str = oss.str();
        log_data["metadata"]["begin_time"] = str.erase(str.size() - 1);
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

    void Task6Logger::log_new_data(size_t optimum_count,
                                   const size_t current_iter) {
        log_data["count_pareto_front"].push_back(optimum_count);
        if (current_iter % print_period == 0) {
            std::cout << std::format(
                             "iteration: {}, individuals on Pareto front: {}",
                             current_iter, optimum_count)
                      << std::endl;
            sync_to_file();
        }
    }
    void Task6Logger::add_final_results(const population_t &population) {
        using individual::operator<<;
        for (const individual_t &individual : population) {
            std::ostringstream oss;
            oss << individual << std::endl;
            std::string result = oss.str();
            if (!result.empty() && result[result.size() - 1] == '\n') {
                result.erase(result.size() - 1);
            }
            log_data["final_population"].push_back(result);
        }
        std::time_t now = std::time(nullptr);
        std::tm *ltm = std::localtime(&now);
        std::ostringstream oss;
        oss << std::put_time(ltm, "%Y-%m-%d %H:%M:%S");
        str = oss.str();
        log_data["metadata"]["end_time"] = str.erase(str.size() - 1);
        std::cout << "Saving log to " << filename << std::endl;
    }
    bool Task6Logger::operator()(population_t &population,
                                 const size_t current_iter) {

        // Count the number of individuals in the Pareto set
        size_t optimum_count = 0;
        for (individual_t &individual : population)
            if (benchmark::is_mlotz_pareto_front(m, individual))
                optimum_count++;

        log_new_data(optimum_count, current_iter);
        if (current_iter >= max_iters || optimum_count == population.size()) {
            if (current_iter >= max_iters)
                std::cout << "Maximum iterations reached..." << std::endl;
            else
                std::cout << "All individuals are on the Pareto front!"
                          << std::endl;

            // save final results
            add_final_results(population);
            sync_to_file();
            return true;
        }
        return false;
    }
} // namespace end_criteria