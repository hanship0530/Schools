package kr.ac.knu.lecture.service;

import kr.ac.knu.lecture.domain.Board;
import kr.ac.knu.lecture.repository.BoardRepository;
import lombok.AllArgsConstructor;
import org.springframework.stereotype.Service;

/**
 * Created by rokim on 2018. 11. 16..
 */
@Service
@AllArgsConstructor
public class BoardService {
    private final BoardRepository boardRepository;

    public Board insertBoard(Board board) {
        return boardRepository.save(board);
    }

    public Board getBoard(long nid) {
        return boardRepository.getOne(nid);
    }
}
