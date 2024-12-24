public class Movie
{
    public int Id { get; set; }
    public string Title { get; set; }
    public string Description { get; set; }
    public int DurationMinutes { get; set; }
    public string Genre { get; set; }
    public ICollection<Screening> Screenings { get; set; }
} 