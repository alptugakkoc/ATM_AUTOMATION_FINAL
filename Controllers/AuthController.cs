using Microsoft.AspNetCore.Mvc;
using System.Security.Cryptography;
using System.Text;

[ApiController]
[Route("api/[controller]")]
public class AuthController : ControllerBase
{
    private readonly CinemaContext _context;
    private readonly IJwtService _jwtService;

    public AuthController(CinemaContext context, IJwtService jwtService)
    {
        _context = context;
        _jwtService = jwtService;
    }

    [HttpPost("register")]
    public async Task<ActionResult<AuthResponse>> Register(RegisterDTO registerDto)
    {
        if (await _context.Users.AnyAsync(u => u.Email == registerDto.Email))
        {
            return BadRequest("Bu email adresi zaten kayıtlı.");
        }

        var user = new User
        {
            Email = registerDto.Email,
            Password = HashPassword(registerDto.Password),
            FirstName = registerDto.FirstName,
            LastName = registerDto.LastName,
            Role = "User"
        };

        _context.Users.Add(user);
        await _context.SaveChangesAsync();

        var token = _jwtService.GenerateToken(user);

        return new AuthResponse
        {
            Token = token,
            Email = user.Email,
            Role = user.Role
        };
    }

    [HttpPost("login")]
    public async Task<ActionResult<AuthResponse>> Login(LoginDTO loginDto)
    {
        var user = await _context.Users
            .FirstOrDefaultAsync(u => u.Email == loginDto.Email);

        if (user == null || user.Password != HashPassword(loginDto.Password))
        {
            return Unauthorized("Geçersiz email veya şifre.");
        }

        var token = _jwtService.GenerateToken(user);

        return new AuthResponse
        {
            Token = token,
            Email = user.Email,
            Role = user.Role
        };
    }

    private string HashPassword(string password)
    {
        using var sha256 = SHA256.Create();
        var hashedBytes = sha256.ComputeHash(Encoding.UTF8.GetBytes(password));
        return Convert.ToBase64String(hashedBytes);
    }
} 