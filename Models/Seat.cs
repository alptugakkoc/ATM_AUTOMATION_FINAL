public class Seat
{
    public int Id { get; set; }
    public string Row { get; set; }
    public int Number { get; set; }
    public int HallId { get; set; }
    public Hall Hall { get; set; }
    public ICollection<Reservation> Reservations { get; set; }
} 