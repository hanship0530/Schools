package kr.ac.knu.lecture.controller.api;

import kr.ac.knu.lecture.domain.User;
import kr.ac.knu.lecture.repository.UserRepository;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.Sort;
import org.springframework.security.core.annotation.AuthenticationPrincipal;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;
import java.util.Optional;

/**
 * Created by rokim on 2018. 11. 30..
 */
@RestController
@RequestMapping("/api/users")
@Slf4j
public class UserController {
    @Autowired
    private UserRepository userRepository;

    @GetMapping("/myself")
    public Optional<User> getMyself(@AuthenticationPrincipal User user) {
        log.info("{}", user);
        Optional<User> playUser = userRepository.findById(user.getName());
        return playUser;
    }
    @GetMapping("/userlists")
    public List<User> getUserLists() {
        return userRepository.findAll(sortByAccountDesc());
    }

    private Sort sortByAccountDesc() {
        return new Sort(Sort.Direction.DESC, "account");
    }

}
