#import <Foundation/Foundation.h>
#import <dlfcn.h>

static NSString* bundleIdentifier = @"ai.paisseon.satella";
static NSMutableDictionary* settings;
static bool enabled;
static bool fakeReceipt;
static bool enableAll;

NSString* satellaReceipt = @"ewoJInNpZ25hdHVyZSIgPSAiQTBMN0Z4UE9lUDBJUGFnd0UrQ3V4bTFNcFZmOE1qVHRvKzdGRGJUTkE5SHhPU1ZVK1h6UVFrcHVxd1RJQzlzZEpMQ2F2S3d6UGpmWUkvOGZXRWJSZldiVFBHUHpIZFZNdHU1clhaOE9JSnNRKy9ySGtMR1lPT3czdmpjdmo3Vk1uRlZOQ2VhRmpjKy9VeWRQVzJxbUlxOHJnUm8rNS9IZGZZTFhTWi8yd1NlcXhlRlR4WVJqRDh0ckdrMjlqajlEcGppNzBjNlFxQlFHaE9nRXB3RzlhSmJJdWFHdnA5OXE1RDlWQjlUSVpVM2FIU3BNa2kwNUdqNkZBellOMG8xQmRkV3VQR3l3d1crdHJBamhyWlhlQVJKc1NwN0xTTzFLRWVjbzNBYk5Od012dE5KL2pLd3AvMlN1UllIL21tdE95ZDF1bzRxUUJQVVhoSXdVUnBtZ0dDUUFBQVdBTUlJRmZEQ0NCR1NnQXdJQkFnSUlEdXRYaCtlZUNZMHdEUVlKS29aSWh2Y05BUUVGQlFBd2daWXhDekFKQmdOVkJBWVRBbFZUTVJNd0VRWURWUVFLREFwQmNIQnNaU0JKYm1NdU1Td3dLZ1lEVlFRTERDTkJjSEJzWlNCWGIzSnNaSGRwWkdVZ1JHVjJaV3h2Y0dWeUlGSmxiR0YwYVc5dWN6RkVNRUlHQTFVRUF3dzdRWEJ3YkdVZ1YyOXliR1IzYVdSbElFUmxkbVZzYjNCbGNpQlNaV3hoZEdsdmJuTWdRMlZ5ZEdsbWFXTmhkR2x2YmlCQmRYUm9iM0pwZEhrd0hoY05NVFV4TVRFek1ESXhOVEE1V2hjTk1qTXdNakEzTWpFME9EUTNXakNCaVRFM01EVUdBMVVFQXd3dVRXRmpJRUZ3Y0NCVGRHOXlaU0JoYm1RZ2FWUjFibVZ6SUZOMGIzSmxJRkpsWTJWcGNIUWdVMmxuYm1sdVp6RXNNQ29HQTFVRUN3d2pRWEJ3YkdVZ1YyOXliR1IzYVdSbElFUmxkbVZzYjNCbGNpQlNaV3hoZEdsdmJuTXhFekFSQmdOVkJBb01Da0Z3Y0d4bElFbHVZeTR4Q3pBSkJnTlZCQVlUQWxWVE1JSUJJakFOQmdrcWhraUc5dzBCQVFFRkFBT0NBUThBTUlJQkNnS0NBUUVBcGMrQi9TV2lnVnZXaCswajJqTWNqdUlqd0tYRUpzczl4cC9zU2cxVmh2K2tBdGVYeWpsVWJYMS9zbFFZbmNRc1VuR09aSHVDem9tNlNkWUk1YlNJY2M4L1cwWXV4c1FkdUFPcFdLSUVQaUY0MWR1MzBJNFNqWU5NV3lwb041UEM4cjBleE5LaERFcFlVcXNTNCszZEg1Z1ZrRFV0d3N3U3lvMUlnZmRZZUZScjZJd3hOaDlLQmd4SFZQTTNrTGl5a29sOVg2U0ZTdUhBbk9DNnBMdUNsMlAwSzVQQi9UNXZ5c0gxUEttUFVockFKUXAyRHQ3K21mNy93bXYxVzE2c2MxRkpDRmFKekVPUXpJNkJBdENnbDdaY3NhRnBhWWVRRUdnbUpqbTRIUkJ6c0FwZHhYUFEzM1k3MkMzWmlCN2o3QWZQNG83UTAvb21WWUh2NGdOSkl3SURBUUFCbzRJQjF6Q0NBZE13UHdZSUt3WUJCUVVIQVFFRU16QXhNQzhHQ0NzR0FRVUZCekFCaGlOb2RIUndPaTh2YjJOemNDNWhjSEJzWlM1amIyMHZiMk56Y0RBekxYZDNaSEl3TkRBZEJnTlZIUTRFRmdRVWthU2MvTVIydDUrZ2l2Uk45WTgyWGUwckJJVXdEQVlEVlIwVEFRSC9CQUl3QURBZkJnTlZIU01FR0RBV2dCU0lKeGNKcWJZWVlJdnM2N3IyUjFuRlVsU2p0ekNDQVI0R0ExVWRJQVNDQVJVd2dnRVJNSUlCRFFZS0tvWklodmRqWkFVR0FUQ0IvakNCd3dZSUt3WUJCUVVIQWdJd2diWU1nYk5TWld4cFlXNWpaU0J2YmlCMGFHbHpJR05sY25ScFptbGpZWFJsSUdKNUlHRnVlU0J3WVhKMGVTQmhjM04xYldWeklHRmpZMlZ3ZEdGdVkyVWdiMllnZEdobElIUm9aVzRnWVhCd2JHbGpZV0pzWlNCemRHRnVaR0Z5WkNCMFpYSnRjeUJoYm1RZ1kyOXVaR2wwYVc5dWN5QnZaaUIxYzJVc0lHTmxjblJwWm1sallYUmxJSEJ2YkdsamVTQmhibVFnWTJWeWRHbG1hV05oZEdsdmJpQndjbUZqZEdsalpTQnpkR0YwWlcxbGJuUnpMakEyQmdnckJnRUZCUWNDQVJZcWFIUjBjRG92TDNkM2R5NWhjSEJzWlM1amIyMHZZMlZ5ZEdsbWFXTmhkR1ZoZFhSb2IzSnBkSGt2TUE0R0ExVWREd0VCL3dRRUF3SUhnREFRQmdvcWhraUc5Mk5rQmdzQkJBSUZBREFOQmdrcWhraUc5dzBCQVFVRkFBT0NBUUVBRGFZYjB5NDk0MXNyQjI1Q2xtelQ2SXhETUlKZjRGelJqYjY5RDcwYS9DV1MyNHlGdzRCWjMrUGkxeTRGRkt3TjI3YTQvdncxTG56THJSZHJqbjhmNUhlNXNXZVZ0Qk5lcGhtR2R2aGFJSlhuWTR3UGMvem83Y1lmcnBuNFpVaGNvT0FvT3NBUU55MjVvQVE1SDNPNXlBWDk4dDUvR2lvcWJpc0IvS0FnWE5ucmZTZW1NL2oxbU9DK1JOdXhUR2Y4YmdwUHllSUdxTktYODZlT2ExR2lXb1IxWmRFV0JHTGp3Vi8xQ0tuUGFObVNBTW5CakxQNGpRQmt1bGhnd0h5dmozWEthYmxiS3RZZGFHNllRdlZNcHpjWm04dzdISG9aUS9PamJiOUlZQVlNTnBJcjdONFl0UkhhTFNQUWp2eWdhWndYRzU2QWV6bEhSVEJoTDhjVHFBPT0iOwoJInB1cmNoYXNlLWluZm8iID0gImV3b0pJbTl5YVdkcGJtRnNMWEIxY21Ob1lYTmxMV1JoZEdVdGNITjBJaUE5SUNJeU1ESXhMVEEzTFRFMUlEQTBPakk1T2pBd0lFRnRaWEpwWTJFdlRHOXpYMEZ1WjJWc1pYTWlPd29KSW5CMWNtTm9ZWE5sTFdSaGRHVXRiWE1pSUQwZ0lqRTBPREF4TkRVNU16UXlNalFpT3dvSkluVnVhWEYxWlMxcFpHVnVkR2xtYVdWeUlpQTlJQ0pXTW1oc1kyMVZaMWxZU214SlNGSnZXbE5DY21KdGJESmFXRTExU1VNd1oxRXlhR2hqYlVWMUlqc0tDU0p2Y21sbmFXNWhiQzEwY21GdWMyRmpkR2x2YmkxcFpDSWdQU0FpTkRRd01EQXdNalkzTVRFMU1EUXhJanNLQ1NKaWRuSnpJaUE5SUNJeUxqVXVNQ0k3Q2draVlYQndMV2wwWlcwdGFXUWlJRDBnSWpFeE1EWTVNelk1TWpFaU93b0pJblJ5WVc1ellXTjBhVzl1TFdsa0lpQTlJQ0kwTkRBd01EQXlOamN4TVRVd05ERWlPd29KSW5GMVlXNTBhWFI1SWlBOUlDSXhJanNLQ1NKdmNtbG5hVzVoYkMxd2RYSmphR0Z6WlMxa1lYUmxMVzF6SWlBOUlDSXhORGd3TVRRMU9UTTBNakkwSWpzS0NTSjFibWx4ZFdVdGRtVnVaRzl5TFdsa1pXNTBhV1pwWlhJaUlEMGdJa1pGUWtReVEwRTFMVUV3TWpJdE5FRkJOQzFDT1VOQ0xUSTBRekJFT1VNeU1FUTNOeUk3Q2draWFYUmxiUzFwWkNJZ1BTQWlNVEV4TkRBeE5UZzBNQ0k3Q2draWRtVnljMmx2YmkxbGVIUmxjbTVoYkMxcFpHVnVkR2xtYVdWeUlpQTlJQ0l3TnpFMU1URXlPU0k3Q2draWNISnZaSFZqZEMxcFpDSWdQU0FpWVdrdWNHRnBjM05sYjI0dWMyRjBaV3hzWVM1amVYQjNiaUk3Q2draWNIVnlZMmhoYzJVdFpHRjBaU0lnUFNBaU1qQXlNUzB3TnkweE5TQXhNVG95T1Rvd01DQkZkR012UjAxVUlqc0tDU0p2Y21sbmFXNWhiQzF3ZFhKamFHRnpaUzFrWVhSbElpQTlJQ0l5TURJeExUQTNMVEUxSURFeE9qSTVPakF3SUVWMFl5OUhUVlFpT3dvSkltSnBaQ0lnUFNBaVlXa3VjR0ZwYzNObGIyNHVjMkYwWld4c1lTSTdDZ2tpY0hWeVkyaGhjMlV0WkdGMFpTMXdjM1FpSUQwZ0lqSXdNakV0TURjdE1UVWdNRFE2TWprNk1EQWdRVzFsY21sallTOU1iM05mUVc1blpXeGxjeUk3Q24wPSI7CgkicG9kIiA9ICI0NCI7Cgkic2lnbmluZy1zdGF0dXMiID0gIjAiOwp9";