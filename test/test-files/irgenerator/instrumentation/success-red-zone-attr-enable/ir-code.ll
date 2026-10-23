; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [11 x i8] c"Value: %d\0A\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z16printWithRedZonei(i32 noundef %0) #0 {
  %value = alloca i32, align 4
  store i32 %0, ptr %value, align 4
  %2 = load i32, ptr %value, align 4
  %3 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %2)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

; Function Attrs: noinline noredzone nounwind optnone uwtable
define internal noundef i32 @_Z19addUpWithoutRedZoneii(i32 noundef %0, i32 noundef %1) #2 {
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 %0, ptr %a, align 4
  store i32 %1, ptr %b, align 4
  %3 = load i32, ptr %b, align 4
  %4 = load i32, ptr %a, align 4
  %5 = add nsw i32 %4, %3
  ret i32 %5
}

; Function Attrs: mustprogress noinline norecurse noredzone nounwind optnone uwtable
define noundef i32 @main() #3 {
  %1 = call noundef i32 @_Z19addUpWithoutRedZoneii(i32 noundef 2, i32 noundef 3)
  call void @_Z16printWithRedZonei(i32 noundef %1)
  ret i32 0
}

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { noinline noredzone nounwind optnone uwtable }
attributes #3 = { mustprogress noinline norecurse noredzone nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
