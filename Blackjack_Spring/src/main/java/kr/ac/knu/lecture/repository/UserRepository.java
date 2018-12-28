package kr.ac.knu.lecture.repository;

import kr.ac.knu.lecture.domain.User;
import org.springframework.data.jpa.repository.JpaRepository;

/**
 * Created by rokim on 2018. 11. 30..
 */
public interface UserRepository extends JpaRepository<User, String> {
}
