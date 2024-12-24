public class Screening
{
    public int Id { get; set; }
    public int MovieId { get; set; }
    public Movie Movie { get; set; }
    public DateTime ScreeningTime { get; set; }
    public int HallId { get; set; }
    public Hall Hall { get; set; }
    public ICollection<Reservation> Reservations { get; set; }
} 