public class Reservation
{
    public int Id { get; set; }
    public int UserId { get; set; }
    public User User { get; set; }
    public int ScreeningId { get; set; }
    public Screening Screening { get; set; }
    public int SeatId { get; set; }
    public Seat Seat { get; set; }
    public DateTime ReservationTime { get; set; }
    public decimal Price { get; set; }
    public string Status { get; set; }
} 