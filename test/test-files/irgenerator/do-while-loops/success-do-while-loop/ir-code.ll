; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [17 x i8] c"i is now at: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store i32 0, ptr %i, align 4
  br label %dowhile.body.L3

dowhile.body.L3:                                  ; preds = %dowhile.foot.L3, %0
  %1 = load i32, ptr %i, align 4
  %2 = add nsw i32 %1, 1
  store i32 %2, ptr %i, align 4
  %3 = load i32, ptr %i, align 4
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %3)
  br label %dowhile.foot.L3

dowhile.foot.L3:                                  ; preds = %dowhile.body.L3
  %5 = load i32, ptr %i, align 4
  %6 = icmp slt i32 %5, 10
  br i1 %6, label %dowhile.body.L3, label %dowhile.exit.L3

dowhile.exit.L3:                                  ; preds = %dowhile.foot.L3
  %7 = load i32, ptr %result, align 4
  ret i32 %7
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
