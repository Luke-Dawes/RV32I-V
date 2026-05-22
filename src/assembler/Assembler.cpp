#include "Assembler.h"
#include <ranges>
#include <string_view>
#include <string>
#include <iostream>
#include <charconv> 
#include <format>


std::vector<uint32_t> Assembler::parse() {

	//find each part split by "\n"
	std::vector<uint32_t> final_code;

	for (auto&& line_range : current | std::views::split('\n')) {

		auto words_view = line_range | std::views::split(' ');
		auto it = words_view.begin();

		if (it == words_view.end()) continue;

		auto first_word_range = *it;
		std::string_view first_word(first_word_range.begin(), first_word_range.end());

		if (first_word.empty()) continue;

		auto base = encoding.find(std::string(first_word));
		if (base == encoding.end()) continue;

		auto instruction = base->second.base_mask;
		auto& type = base->second.format;


		switch (type) {

			case Format::I_TYPE: {
				if (++it == words_view.end()) continue;
				auto rd_range = *it;
				std::string_view rd_str(rd_range.begin(), rd_range.end());
				if (!rd_str.empty() && rd_str.back() == ',') {
					rd_str.remove_suffix(1);
				}

				if (++it == words_view.end()) continue;
				auto rs_range = *it;
				std::string_view rs_str(rs_range.begin(), rs_range.end());
				if (!rs_str.empty() && rs_str.back() == ',') {
					rs_str.remove_suffix(1);
				}

				if (++it == words_view.end()) continue;
				auto imm_range = *it;
				std::string_view imm_str(imm_range.begin(), imm_range.end());

				int immediate_value = 0;
				auto [ptr, ec] = std::from_chars(imm_str.data(), imm_str.data() + imm_str.size(), immediate_value);
					
				if (ec != std::errc()) {
					// Handle malformed immediate error here
					continue;
				}

				auto rd_it = register_to_number.find(std::string(rd_str));
				auto rs1_it = register_to_number.find(std::string(rs_str));

				if (rd_it == register_to_number.end() || rs1_it == register_to_number.end()) {
					// Handle error: Unknown register name
					continue;
				}


				uint32_t rd_num = rd_it->second;
				uint32_t rs1_num = rs1_it->second;


				instruction |= (rd_num << 7);
				instruction |= (rs1_num << 15);
				instruction |= ((static_cast<uint32_t>(immediate_value) & 0xFFF) << 20);

				final_code.push_back(instruction);

		}
				break;

			case Format::R_TYPE: {
				if (++it == words_view.end()) continue;
				auto rd_range = *it;
				std::string_view rd_str(rd_range.begin(), rd_range.end());
				if (!rd_str.empty() && rd_str.back() == ',') {
					rd_str.remove_suffix(1);
				}

				if (++it == words_view.end()) continue;
				auto rs1_range = *it;
				std::string_view rs1_str(rs1_range.begin(), rs1_range.end());
				if (!rs1_str.empty() && rs1_str.back() == ',') {
					rs1_str.remove_suffix(1);
				}
				if (++it == words_view.end()) continue;
				auto rs2_range = *it;
				std::string_view rs2_str(rs2_range.begin(), rs2_range.end());
				if (!rs2_str.empty() && rs2_str.back() == ',') {
					rs2_str.remove_suffix(1);
				}

				auto rd_it = register_to_number.find(std::string(rd_str));
				auto rs1_it = register_to_number.find(std::string(rs1_str));
				auto rs2_it = register_to_number.find(std::string(rs2_str));

				if (rd_it == register_to_number.end() || rs1_it == register_to_number.end() || rs2_it == register_to_number.end()) {
					// Handle error: Unknown register name
					continue;
				}

				uint32_t rd_num = rd_it->second;
				uint32_t rs1_num = rs1_it->second;
				uint32_t rs2_num = rs2_it->second;

				instruction |= (rd_num << 7);
				instruction |= (rs1_num << 15);
				instruction |= (rs2_num << 20);

				final_code.push_back(instruction);

			}
				break;
			
			
			
			

			default:
				break;
		}

	}
	
	for (auto& t : final_code) {
		std::cout << std::format("0x{:08x}\n", t) << "\n";

	}
	return final_code;
}