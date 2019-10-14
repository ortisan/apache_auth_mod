package ortiz.sts.util;

import com.auth0.jwt.JWT;
import com.auth0.jwt.algorithms.Algorithm;
import com.auth0.jwt.interfaces.DecodedJWT;
import com.auth0.jwt.interfaces.JWTVerifier;
import ortiz.sts.model.User;

public class JwtUtil {

    private static final String SECRET = "aloha";
    private static final String USERNAME = "username";
    private static final String USER_ID = "USER_ID";


    public static User getUserFromToken(String token) {
        JWTVerifier verifier = JWT.require(Algorithm.HMAC256(SECRET)).build();
        DecodedJWT decodedJWT = verifier.verify(token);
        String userId = decodedJWT.getClaim(USER_ID).asString();
        String username = decodedJWT.getClaim(USERNAME).asString();
        User user = new User();
        user.setId(userId);
        user.setUsername(username);
        return user;
    }

    public static String getTokenFromUser(User user) {
        return JWT.create().withClaim(USER_ID, user.getId())
                .withClaim(USERNAME, user.getUsername())
                .sign(Algorithm.HMAC256(SECRET));

    }

}
