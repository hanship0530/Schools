package kr.ac.knu.lecture.repository;

import lombok.AllArgsConstructor;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;

import javax.sql.DataSource;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;

/**
 * Created by rokim on 2018. 11. 16..
 */
@Repository
@AllArgsConstructor
public class JdbcTemplateRepository {
    private final DataSource dataSource;
    private final JdbcTemplate jdbcTemplate;

    public String getTitleById(long nid) throws SQLException {
Connection connection = dataSource.getConnection();
PreparedStatement pst = connection.prepareStatement("SELECT title FROM board where id = ?", new String[]{String.valueOf(nid)});

pst.executeQuery();

return jdbcTemplate
        .queryForObject(
                "SELECT title " +
                        "FROM board " +
                        "LIMIT 10",
                new Object[] {nid}, String.class);
    }
}
