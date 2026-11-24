; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [8 x i8] c"abs: %d\00", align 4
@printf.str.0 = private unnamed_addr constant [15 x i8] c"%s (%d chars)\0A\00", align 4

; Function Attrs: nounwind
declare noundef i32 @abs(i32 noundef) #0

; Function Attrs: nounwind
declare noundef ptr @malloc(i64 noundef) #0

; Function Attrs: nounwind
declare void @free(ptr noundef) #0

; Function Attrs: nounwind
declare noundef i32 @snprintf(ptr noundef, i64 noundef, ptr noundef, ...) #0

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define noundef i32 @main() #1 {
  %address = alloca ptr, align 8
  %length = alloca i32, align 4
  %1 = call noundef ptr @malloc(i64 noundef 32)
  store ptr %1, ptr %address, align 8
  %2 = load ptr, ptr %address, align 8
  %3 = call noundef i32 @abs(i32 noundef -42)
  %4 = call noundef i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef align 1 %2, i64 noundef 32, ptr noundef @anon.string.0, i32 noundef %3)
  store i32 %4, ptr %length, align 4
  %5 = load ptr, ptr %address, align 8
  %6 = load i32, ptr %length, align 4
  %7 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr noundef %5, i32 noundef %6)
  %8 = load ptr, ptr %address, align 8
  call void @free(ptr noundef align 1 %8)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

attributes #0 = { nounwind }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #2 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
