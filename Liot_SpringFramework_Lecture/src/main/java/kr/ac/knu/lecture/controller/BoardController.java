package kr.ac.knu.lecture.controller;

import kr.ac.knu.lecture.domain.Board;
import kr.ac.knu.lecture.repository.JdbcTemplateRepository;
import kr.ac.knu.lecture.service.BoardService;
import lombok.AllArgsConstructor;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.servlet.ModelAndView;

import java.sql.SQLException;

/**
 * Created by rokim on 2018. 11. 16..
 */
@Controller
@AllArgsConstructor
@RequestMapping("/board-service")
public class BoardController {
    private final BoardService boardService;

    @GetMapping("/insertBoard")
    @ResponseBody
    public Board insertBoard(String title, String content) {
        Board board = new Board();
        board.setTitle(title);
        board.setContent(content);
        return boardService.insertBoard(board);
    }

    private final JdbcTemplateRepository jdbcTemplateRepository;

    @GetMapping("/detail")
    public String getBoard(Model model, long nid) throws SQLException {
        Board board = boardService.getBoard(nid);

        System.out.println(jdbcTemplateRepository.getTitleById(nid));

        model.addAttribute("board", board);
        return "detail";
    }
}
