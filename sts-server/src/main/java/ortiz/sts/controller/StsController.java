package ortiz.sts.controller;

import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;
import ortiz.sts.model.User;
import ortiz.sts.util.JwtUtil;

@RestController
public class StsController {

    @RequestMapping("/token")
    public String getToken() {
        final User user = new User();
        user.setId("1");
        user.setUsername("tentativafc");
        return JwtUtil.getTokenFromUser(user);
    }

    @RequestMapping("/token_info")
    public ResponseEntity<User> getUser(@RequestParam(value = "token") String token) {
        try {
            User user = JwtUtil.getUserFromToken(token);
            return new ResponseEntity(user, HttpStatus.OK);
        } catch (Exception exc) {
            return new ResponseEntity(null, HttpStatus.UNAUTHORIZED);
        }
    }

    @RequestMapping("/authorized_hello")
    public String authorized() {
        return "This request was authorized";
    }

}
