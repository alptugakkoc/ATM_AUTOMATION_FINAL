public class LoginDTO
{
    public string Email { get; set; }
    public string Password { get; set; }
}

public class RegisterDTO
{
    public string Email { get; set; }
    public string Password { get; set; }
    public string FirstName { get; set; }
    public string LastName { get; set; }
}

public class AuthResponse
{
    public string Token { get; set; }
    public string Email { get; set; }
    public string Role { get; set; }
} 