; ModuleID = 'source.spice'
source_filename = "source.spice"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
while.body.L5:
  %putchar = tail call i32 @putchar(i32 72)
  %putchar.1 = tail call i32 @putchar(i32 101)
  %putchar.2 = tail call i32 @putchar(i32 108)
  %putchar.3 = tail call i32 @putchar(i32 108)
  %putchar.4 = tail call i32 @putchar(i32 111)
  %putchar.5 = tail call i32 @putchar(i32 32)
  %putchar.6 = tail call i32 @putchar(i32 87)
  %putchar.7 = tail call i32 @putchar(i32 111)
  %putchar.8 = tail call i32 @putchar(i32 114)
  %putchar.9 = tail call i32 @putchar(i32 108)
  %putchar.10 = tail call i32 @putchar(i32 100)
  %putchar.11 = tail call i32 @putchar(i32 32)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @putchar(i32 noundef) local_unnamed_addr #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
