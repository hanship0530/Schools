package kr.ac.knu.lecture.config;

/**
 * Created by rokim on 2018. 11. 30..
 */

import kr.ac.knu.lecture.domain.OAuthProvider;
import kr.ac.knu.lecture.domain.User;
import kr.ac.knu.lecture.repository.UserRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.autoconfigure.security.oauth2.resource.PrincipalExtractor;
import org.springframework.boot.autoconfigure.security.oauth2.resource.ResourceServerProperties;
import org.springframework.boot.autoconfigure.security.oauth2.resource.UserInfoTokenServices;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.boot.web.servlet.FilterRegistrationBean;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.WebSecurityConfigurerAdapter;
import org.springframework.security.oauth2.client.OAuth2ClientContext;
import org.springframework.security.oauth2.client.OAuth2RestTemplate;
import org.springframework.security.oauth2.client.filter.OAuth2ClientAuthenticationProcessingFilter;
import org.springframework.security.oauth2.client.filter.OAuth2ClientContextFilter;
import org.springframework.security.oauth2.client.token.grant.code.AuthorizationCodeResourceDetails;
import org.springframework.security.oauth2.config.annotation.web.configuration.EnableOAuth2Client;
import org.springframework.security.web.authentication.LoginUrlAuthenticationEntryPoint;
import org.springframework.security.web.authentication.www.BasicAuthenticationFilter;
import org.springframework.web.filter.CompositeFilter;

import javax.servlet.Filter;
import java.util.Arrays;
import java.util.List;
import java.util.Map;
import java.util.Optional;

/**
 * Created by rokim on 2018. 11. 30..
 */
@Configuration
@EnableOAuth2Client
public class SecurityConfiguration extends WebSecurityConfigurerAdapter {
    @Autowired
    private OAuth2ClientContext oauth2ClientContext;

    @Override
    protected void configure(HttpSecurity http) throws Exception {
        http
                .cors()
                .and().csrf().disable()
                .antMatcher("/**").authorizeRequests()
                .antMatchers("/", "/view/**", "/login**", "/webjars/**", "/error**" ,"/blackjack/**", "/h2-console/**")
                .permitAll().anyRequest().authenticated()
                .and()
                .exceptionHandling().authenticationEntryPoint(new LoginUrlAuthenticationEntryPoint("/"))
                .and()
                .logout().logoutSuccessUrl("/blackjack/index.html").permitAll()
                .and()
                .headers().frameOptions().disable()
                .and()
                .addFilterBefore(ssoFilter(), BasicAuthenticationFilter.class);
    }

    private Filter ssoFilter() {
        CompositeFilter compositeFilter = new CompositeFilter();
        compositeFilter.setFilters(Arrays.asList(githubSsoFilter(), naverSsoFilter()));
        return compositeFilter;
    }

    private Filter githubSsoFilter() {
        OAuth2ClientAuthenticationProcessingFilter githubFilter = new OAuth2ClientAuthenticationProcessingFilter("/login/github");
        OAuth2RestTemplate githubTemplate = new OAuth2RestTemplate(github(), oauth2ClientContext);
        githubFilter.setRestTemplate(githubTemplate);
        UserInfoTokenServices tokenServices = new UserInfoTokenServices(githubResource().getUserInfoUri(), github().getClientId());
        tokenServices.setRestTemplate(githubTemplate);
        tokenServices.setPrincipalExtractor(githubPrincipalExtractor());
        githubFilter.setTokenServices(tokenServices);
        githubFilter.setAuthenticationSuccessHandler((httpServletRequest, httpServletResponse, authentication) -> httpServletResponse.sendRedirect("/blackjack/index.html"));

        return githubFilter;
    }

    private Filter naverSsoFilter() {
        OAuth2ClientAuthenticationProcessingFilter naverFilter = new OAuth2ClientAuthenticationProcessingFilter("/login/naver");
        OAuth2RestTemplate naverTemplate = new OAuth2RestTemplate(naver(), oauth2ClientContext);
        naverFilter.setRestTemplate(naverTemplate);
        UserInfoTokenServices tokenServices = new UserInfoTokenServices(naverResource().getUserInfoUri(), naver().getClientId());
        tokenServices.setRestTemplate(naverTemplate);
        tokenServices.setPrincipalExtractor(naverPrincipalExtractor());
        naverFilter.setTokenServices(tokenServices);
        naverFilter.setAuthenticationSuccessHandler((httpServletRequest, httpServletResponse, authentication) -> httpServletResponse.sendRedirect("/blackjack/index.html"));

        return naverFilter;

    }

    @Bean
    public FilterRegistrationBean oauth2ClientFilterRegistration(
            OAuth2ClientContextFilter filter) {
        FilterRegistrationBean registration = new FilterRegistrationBean();
        registration.setFilter(filter);
        registration.setOrder(-100);
        return registration;
    }

    @Bean
    @ConfigurationProperties("github.client")
    public AuthorizationCodeResourceDetails github() {
        return new AuthorizationCodeResourceDetails();
    }

    @Bean
    @ConfigurationProperties("github.resource")
    public ResourceServerProperties githubResource() {
        return new ResourceServerProperties();
    }

    @Bean
    @ConfigurationProperties("naver.client")
    public AuthorizationCodeResourceDetails naver() {
        return new AuthorizationCodeResourceDetails();
    }

    @Bean
    @ConfigurationProperties("naver.resource")
    public ResourceServerProperties naverResource() {
        return new ResourceServerProperties();
    }

    @Autowired
    private UserRepository userRepository;

    public PrincipalExtractor githubPrincipalExtractor() {
        return (map) -> {
            String loginId = (String) map.get("login");
            Optional<User> user = userRepository.findById(loginId);
            if (user.isPresent()) {
                return user.get();
            }

            User newUser = new User((String) map.get("login"), OAuthProvider.GITHUB, String.valueOf(map.get("id")), 50000L);;
            return userRepository.save(newUser);
        };
    }

    private PrincipalExtractor naverPrincipalExtractor() {
        return (map) -> {
            Map<String, String> response = (Map<String, String>) map.get("response");
            String name = response.get("name");
            String id = response.get("id");
            Optional<User> user = userRepository.findById(name);
            if (user.isPresent()) {
                return user.get();
            }

            User newUser = new User(name, OAuthProvider.NAVER, id, 60000L);
            return userRepository.save(newUser);
        };
    }

}
