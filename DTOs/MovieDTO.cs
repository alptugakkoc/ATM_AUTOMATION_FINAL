public class MovieDTO
{
    public int Id { get; set; }
    public string Title { get; set; }
    public string Description { get; set; }
    public int DurationMinutes { get; set; }
    public string Genre { get; set; }
}

public class CreateMovieDTO
{
    public string Title { get; set; }
    public string Description { get; set; }
    public int DurationMinutes { get; set; }
    public string Genre { get; set; }
} 