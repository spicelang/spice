; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [15 x i8] c"Hello syscall!\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %str = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  store ptr @anon.string.0, ptr %str, align 8
  %1 = load ptr, ptr %str, align 8
  %2 = ptrtoint ptr %1 to i64
  %3 = load ptr, ptr %str, align 8
  %4 = call i64 @_Z12getRawLengthPKc(ptr %3)
  call void asm sideeffect "movq $0, %rax\0Amovq $1, %rdi\0Amovq $2, %rsi\0Amovq $3, %rdx\0Amovq $4, %r10\0Amovq $5, %r8\0Amovq $6, %r9\0Asyscall\0A", "r,r,r,r,r,r,r,~{rax},~{rdi},~{rsi},~{rdx},~{r10},~{r8},~{r9},~{dirflag},~{fpsr},~{flags}"(i64 1, i64 1, i64 %2, i64 %4, i64 0, i64 0, i64 0)
  %5 = load i32, ptr %result, align 4
  ret i32 %5
}

declare i64 @_Z12getRawLengthPKc(ptr)

attributes #0 = { noinline nounwind optnone uwtable }
