package kr.ac.knu.lecture.controller;

import kr.ac.knu.lecture.repository.BoardRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

/**
 * Created by rokim on 2018. 11. 16..
 */
@Controller
@RequestMapping("/view")
public class ViewController {
    @Autowired
    private BoardRepository boardRepository;

    @GetMapping("/hello")
    public String hello(Model model,
                        @RequestParam(name = "name") String name,
                        @RequestParam int code,
                        @RequestParam long nid) {
        model.addAttribute("name", name);
        return "hello";
    }
}
