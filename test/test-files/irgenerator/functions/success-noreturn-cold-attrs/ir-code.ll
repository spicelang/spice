; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [11 x i8] c"Error: %s\0A\00", align 4
@anon.string.0 = private unnamed_addr constant [17 x i8] c"Unexpected value\00", align 4
@printf.str.1 = private unnamed_addr constant [11 x i8] c"Value: %d\0A\00", align 4

; Function Attrs: cold noreturn nounwind
declare void @exit(i32 noundef) #0

; Function Attrs: cold noinline nounwind optnone uwtable
define internal void @_Z11reportErrorPKc(ptr noundef %0) #1 {
  %message = alloca ptr, align 8
  store ptr %0, ptr %message, align 8
  %2 = load ptr, ptr %message, align 8
  %3 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr noundef %2)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_Z8getValuev() #3 {
  ret i32 21
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define noundef i32 @main() #4 {
  %value = alloca i32, align 4
  %1 = call noundef i32 @_Z8getValuev()
  %2 = mul nsw i32 %1, 2
  store i32 %2, ptr %value, align 4
  %3 = load i32, ptr %value, align 4
  %4 = icmp ne i32 %3, 42
  br i1 %4, label %if.then.L21, label %if.exit.L21

if.then.L21:                                      ; preds = %0
  call void @_Z11reportErrorPKc(ptr noundef @anon.string.0)
  %5 = load i32, ptr %value, align 4
  call void @_Z13reportFailurei(i32 noundef %5)
  br label %if.exit.L21

if.exit.L21:                                      ; preds = %if.then.L21, %0
  %6 = load i32, ptr %value, align 4
  %7 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 noundef %6)
  call void @_Z9terminatei(i32 noundef 0)
  ret i32 0
}

; Function Attrs: cold
declare void @_Z13reportFailurei(i32) #5

; Function Attrs: noreturn
declare void @_Z9terminatei(i32) #6

attributes #0 = { cold noreturn nounwind }
attributes #1 = { cold noinline nounwind optnone uwtable }
attributes #2 = { nofree nounwind }
attributes #3 = { noinline nounwind optnone uwtable }
attributes #4 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #5 = { cold }
attributes #6 = { noreturn }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
