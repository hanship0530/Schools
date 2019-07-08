package kr.ac.knu.lecture.repository;

import kr.ac.knu.lecture.domain.Student;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

/**
 * Created by rokim on 2018. 11. 16..
 */
@Repository
public interface StudentRepository extends JpaRepository<Student, Long>{
}
