#include "Assembler.h"
#include <ranges>
#include <string_view>
#include <string>
#include <iostream>
#include <charconv> 
#include <format>


std::vector<uint32_t> Assembler::parse() { //this needs a symbol table for like labels for the jump instructions

	//look through the code and find any labels - found by .whatever, and calculate what byte they are 
	parse_into_vector();
	first_pass();
	curr_PC = 0;

	//find each part split by "\n"
	std::vector<uint32_t> final_code;

	for (auto&& line_range : current | std::views::split('\n')) {

		auto words_view = line_range | std::views::split(' ');
		auto it = words_view.begin();

		if (it == words_view.end()) continue;

		auto first_word_range = *it;
		std::string_view first_word(first_word_range.begin(), first_word_range.end());

		if (first_word.empty()) continue;

		if (first_word.front() == '#') continue;

		size_t actual_text_start = first_word.find_first_not_of(" \t");

		if (actual_text_start == std::string::npos) {
			continue;
		}

		first_word.remove_prefix(actual_text_start);

		//if (!encoding.count((std::string)first_word)) continue;

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

				std::cout << (std::string)rd_str << " " << (std::string) rs_str << " " << (std::string)imm_str << "\n";

				int immediate_value = 0;
				auto [ptr, ec] = std::from_chars(imm_str.data(), imm_str.data() + imm_str.size(), immediate_value);
					
				if (ec != std::errc()) {
					// Handle malformed immediate error here
					std::cerr << "Assembler Error: Malformed immediate [" << (std::string)imm_str << "]\n";

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
				curr_PC += 4;

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
				curr_PC += 4;

			}
				break;
			
			case Format::S_TYPE: { //correct syntax is rs2, imm(rs1)

				if (++it == words_view.end()) continue;
				auto rs2_range = *it;
				std::string_view rs2_str(rs2_range.begin(), rs2_range.end());
				if (!rs2_str.empty() && rs2_str.back() == ',') {
					rs2_str.remove_suffix(1);
				}

				if (++it == words_view.end()) continue;
				auto mem_range = *it;
				std::string_view mem_str(mem_range.begin(), mem_range.end());

				size_t open_paren = mem_str.find('(');
				size_t close_paren = mem_str.find(')');
				if (open_paren == std::string_view::npos || close_paren == std::string_view::npos || close_paren < open_paren) {
					// Handle malformed memory syntax error
					continue;
				}

				std::string_view imm_str = mem_str.substr(0, open_paren);
				std::string_view rs1_str = mem_str.substr(open_paren + 1, close_paren - open_paren - 1);

				int immediate_value = 0;
				auto [ptr, ec] = std::from_chars(imm_str.data(), imm_str.data() + imm_str.size(), immediate_value);
				if (ec != std::errc()) {
					continue;
				}

				auto rs1_it = register_to_number.find(std::string(rs1_str));
				auto rs2_it = register_to_number.find(std::string(rs2_str));
				if (rs1_it == register_to_number.end() || rs2_it == register_to_number.end()) {
					continue;
				}


				uint32_t rs1_num = rs1_it->second;
				uint32_t rs2_num = rs2_it->second;

				instruction |= (rs1_num << 15);
				instruction |= (rs2_num << 20);
				instruction |= ((immediate_value & 0x1F) << 7);
				instruction |= (((immediate_value >> 5) & 0x7F) << 25);

				final_code.push_back(instruction);
				curr_PC += 4;
			}
				break;

			case Format::B_TYPE: {

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

				if (++it == words_view.end()) continue;
				auto imm_range = *it;
				std::string_view imm_str(imm_range.begin(), imm_range.end());
				int immediate_value = 0;

				if (!symbol_table.contains((std::string)imm_str)) {


					auto [ptr, ec] = std::from_chars(imm_str.data(), imm_str.data() + imm_str.size(), immediate_value);

					if (ec != std::errc()) {
						// Handle malformed immediate error here
						continue;
					}
				}
				else
				{
					immediate_value = symbol_table[(std::string)imm_str] - curr_PC;
				}

				auto rs1_it = register_to_number.find(std::string(rs1_str));
				auto rs2_it = register_to_number.find(std::string(rs2_str));

				if (rs1_it == register_to_number.end() || rs2_it == register_to_number.end()) {
					// Handle error: Unknown register name
					continue;
				}

				uint32_t rs1_num = rs1_it->second;
				uint32_t rs2_num = rs2_it->second;
				immediate_value >>= 1;

				instruction |= (rs1_num << 15);
				instruction |= (rs2_num << 20);
				instruction |= (((immediate_value >> 11) & 1) << 7);
				instruction |= (((immediate_value >> 1) & 0xF) << 8);
				instruction |= (((immediate_value >> 5) & 0x3F) << 25);
				instruction |= (((immediate_value >> 12) & 1) << 31);

				final_code.push_back(instruction);
				curr_PC += 4;

			}
				break;

			case Format::U_TYPE: {

				if (++it == words_view.end()) continue;
				auto rd_range = *it;
				std::string_view rd_str(rd_range.begin(), rd_range.end());
				if (!rd_str.empty() && rd_str.back() == ',') {
					rd_str.remove_suffix(1);
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

				if (rd_it == register_to_number.end()) {
					// Handle error: Unknown register name
					continue;
				}

				uint32_t rd_num = rd_it->second;

				

				instruction |= (rd_num << 7);
				instruction |= (((immediate_value >> 12) & 0xFFFFF) << 12);
				final_code.push_back(instruction);
				curr_PC += 4;

			}
				break;

			case Format::J_TYPE: {

				if (++it == words_view.end()) continue;
				auto rd_range = *it;
				std::string_view rd_str(rd_range.begin(), rd_range.end());
				if (!rd_str.empty() && rd_str.back() == ',') {
					rd_str.remove_suffix(1);
				}

				if (++it == words_view.end()) continue;
				auto imm_range = *it;
				std::string_view imm_str(imm_range.begin(), imm_range.end());
				int immediate_value = 0;

				if (!symbol_table.contains((std::string)imm_str)) {

					
					auto [ptr, ec] = std::from_chars(imm_str.data(), imm_str.data() + imm_str.size(), immediate_value);

					if (ec != std::errc()) {
						// Handle malformed immediate error here
						continue;
					}
				} 
				else
				{
					immediate_value = symbol_table[(std::string)imm_str] - curr_PC;
				}


				auto rd_it = register_to_number.find(std::string(rd_str));

				if (rd_it == register_to_number.end()) {
					// Handle error: Unknown register name
					continue;
				}

				uint32_t rd_num = rd_it->second;

				instruction |= (rd_num << 7);

				immediate_value >>= 1;

				instruction |= (((immediate_value >> 12) & 0x7F) << 12);
				instruction |= (((immediate_value >> 11) & 1) << 20);
				instruction |= (((immediate_value >> 1) & 0x3FF) << 21);
				instruction |= (((immediate_value >> 20) & 1) << 31);

				final_code.push_back(instruction);
				curr_PC += 4;
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

void Assembler::first_pass() {
	//loop vairables have to be in their own thing, i.e. 

	

	uint32_t pc = 0;

	for (const std::string& line : text) {
		if (line.empty() || line.front() == '#') {
			continue;
		}
		if (size_t colon = line.find(':'); colon != std::string_view::npos) {
			std::string label_name = line.substr(0, colon);
			std::cout << label_name << " " << pc << "\n";
			symbol_table[label_name] = pc;
			continue;
		}
		pc += 4;
	}


	/*
	for (auto it = current.begin(); it != current.end(); ++it) {
		std::string_view line((*it).data, (*it).size());

		size_t start = line.find_first_not_of(" \t");
		if (start == std::string_view::npos || line[start] == '#') continue;
		line.remove_prefix(start);

		//finds colon and assigns it and then checks in the if statement if it exists.
		if (size_t colon = line.find(':'); colon != std::string_view::npos) {
			std::string_view label_name = line.substr(0, colon);
			symbol_table[label_name] = pc;
			continue;
		}
		pc += 4;
	}

	*/
	/*
	for (auto&& line_range : current | std::views::split('\n')) {

		auto words_view = line_range | std::views::split(' ');
		auto it = words_view.begin();

		if (it == words_view.end()) continue;

		auto first_word_range = *it;
		std::string_view first_word(first_word_range.begin(), first_word_range.end());

		if (first_word.empty()) continue;

		if (first_word.front() == '#') continue;

		if (encoding.count(std::string(first_word)))
		{
			curr_PC += 4;
			continue;
		}

		std::string word = std::string(first_word);
		if (word.back() == ':') {
			word = word.substr(0, word.size() - 1);
		}

		std::cout << word << " " << curr_PC << "\n";

		symbol_table.insert({ word, curr_PC });
		//curr_PC += 4;
	}
	*/
}

void Assembler::parse_into_vector() {

	std::string_view text_view(current);

	while (!text_view.empty()) {

		

		size_t newline_pos = text_view.find('\n');
		std::string_view line = (newline_pos == std::string_view::npos)
			? text_view
			: text_view.substr(0, newline_pos);

		if (!line.empty() && line.back() == '\r') {
			line.remove_suffix(1);
		}

		size_t first_real_char = line.find_first_not_of(" \t");
		if (first_real_char != std::string_view::npos) {
			line.remove_prefix(first_real_char);
		}
		else {
			line = ""; // The line was entirely spaces, make it empty
		}

		if (line != "") {
			std::cout << line << "\n";
			text.emplace_back(line);
		}

		if (newline_pos == std::string_view::npos) {
			break;
		}
		text_view.remove_prefix(newline_pos + 1);
	}
}